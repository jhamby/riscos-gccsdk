/* GDK - The GIMP Drawing Kit
 * gdkdisplay-riscos.c
 * 
 * Copyright 2001 Sun Microsystems Inc.
 * Copyright (C) 2004 Nokia Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Written by Lee Noar using code and guidance from other targets.
 */

#include "config.h"

#include "gdkdisplay-riscos.h"

#include "gdkdisplay.h"
#include "gdkinternals.h"
#include "gdkscreen-riscos.h"
#include "gdkdevicemanager-riscos.h"
#include "gdkmonitor-riscos.h"
#include "gdkwindow-riscos.h"
#include "gdkprivate-riscos.h"

#include "cairo-riscos.h"

#include "gdkriscos.h"

#include "oslib/os.h"
#include "oslib/toolbox.h"
#define UNKNOWN 1
#include "oslib/colourtrans.h"

#include <string.h>

#define DATA_FORMAT BGR ((os_mode_flags) 0x0u)
#define DATA_FORMAT_RGB ((os_mode_flags) 0x4u)

static void   gdk_riscos_display_dispose            (GObject            *object);
static void   gdk_riscos_display_finalize           (GObject            *object);

G_DEFINE_TYPE (GdkRiscosDisplay, gdk_riscos_display, GDK_TYPE_DISPLAY)


static void
gdk_riscos_display_init (GdkRiscosDisplay *display)
{
  _gdk_riscos_display_manager_add_display (gdk_display_manager_get (),
					   GDK_DISPLAY_OBJECT (display));

  display->monitor = g_object_new (GDK_TYPE_RISCOS_MONITOR,
                                   "display", display,
                                   NULL);

  display->id_ht = g_hash_table_new (NULL, NULL);
  display->toplevels = NULL;

  display->raw_event_handlers = NULL;
  display->message_handlers = NULL;

  display->last_mouse_move.x = -1;
  display->last_mouse_move.y = -1;

  display->last_click.w = 0;
  display->system_drag = FALSE;

  display->key_down_list = NULL;

  display->focus_window = NULL;
  display->selection_window = NULL;

  display->selection.buffer = NULL;
  display->selection.max_size = 0;
  display->selection.size = 0;
  display->selection.claimed = FALSE;

  display->clipboard.buffer = NULL;
  display->clipboard.max_size = 0;
  display->clipboard.size = 0;
  display->clipboard.claimed = FALSE;

  display->clipboard_xfer_progress = NULL;

  display->keymap = NULL;

  display->trans_table = NULL;
  display->render_scale_ptr = NULL;

  display->tb_block = NULL;
  display->tb_messages = NULL;
  display->resource_dir_name = NULL;

  gdk_monitor_set_manufacturer (display->monitor, "RISC OS");
  gdk_monitor_set_model (display->monitor, "0");
}

static void
gdk_event_init (GdkDisplay *display)
{
  GdkRiscosDisplay *riscos_display;

  riscos_display = GDK_RISCOS_DISPLAY (display);
  riscos_display->next_serial = 1;
  riscos_display->last_seen_time = 0;
  _gdk_riscos_events_init (display);
}

static void
gdk_riscos_wimp_init (GdkRiscosDisplay *display)
{
  wimp_MESSAGE_LIST(2) message_list = { { 0 } };
  toolbox_ACTION_LIST(2) action_list = { { 0 } };

  os_error *err;

  display->task_name = g_get_prgname ();
  if (display->task_name == NULL)
    g_error ("Could not determine name of task to initialise") ;

  if (display->resource_dir_name) {
    err = xtoolbox_initialise (0,
			       wimp_VERSION_RO38,
			       (wimp_message_list *)&message_list,
			       (toolbox_action_list *)&action_list,
			       display->resource_dir_name,
			       display->tb_messages,
			       display->tb_block,
			       NULL,
			       &display->task_handle,
			       NULL);
  } else {
    err = xwimp_initialise (wimp_VERSION_RO38,
			    display->task_name,
			    (wimp_message_list *)&message_list,
			    NULL,
			    &display->task_handle);
  }
  if (err != NULL)
    g_error ("Failed to initialise task; %s", err->errmess);
}

GdkDisplay *
_gdk_riscos_display_open (const gchar *display_name)
{
  GdkRiscosDisplay *riscos_display;

  _gdk_display = g_object_new (GDK_TYPE_RISCOS_DISPLAY, NULL);
  riscos_display = GDK_RISCOS_DISPLAY (_gdk_display);

  riscos_display->tb_block = _tb_block;
  riscos_display->tb_messages = _tb_messages;
  riscos_display->resource_dir_name = _resource_dir_name;

  gdk_riscos_wimp_init (riscos_display);

  /* Initialize the display's screens */
  riscos_display->screens = g_new (GdkScreen *, 1);
  riscos_display->screens[0] = _gdk_riscos_screen_new (_gdk_display, 0);

  /*set the default screen */
  riscos_display->default_screen = riscos_display->screens[0];

  _gdk_display->device_manager = _gdk_riscos_device_manager_new (_gdk_display);

  gdk_event_init (_gdk_display);

  _gdk_riscos_screen_setup (riscos_display->screens[0]);

  _gdk_riscos_device_manager_add_seat (_gdk_display->device_manager);

  if (swap_redblue)
    cairo_riscos_swap_red_blue(swap_redblue == swap_redblue_YES);
  else
    {
      int mode_flags = 0;
      int log2bpp;
      xos_read_mode_variable(os_CURRENT_MODE,
			     os_MODEVAR_MODE_FLAGS,
			     &mode_flags,
			     NULL);
      xos_read_mode_variable(os_CURRENT_MODE,
			     os_VDUVAR_LOG2_BPP,
			     &log2bpp,
			     NULL);
      int data_format = (mode_flags & os_MODE_FLAG_DATA_FORMAT) >> os_MODE_FLAG_DATA_FORMAT_SHIFT;
      if ((data_format & DATA_FORMAT_RGB) && log2bpp != 4 && log2bpp != 5)
	cairo_riscos_swap_red_blue(TRUE);
/*      int mode_flags = 0;
      xos_read_mode_variable(os_CURRENT_MODE,
			     os_MODEVAR_MODE_FLAGS,
			     &mode_flags,
			     NULL);
      if ((mode_flags & (3 << os_MODE_FLAG_DATA_FORMAT_SHIFT)) == 0)
	{
	  if (((mode_flags >> 14) & 1))
	    cairo_riscos_swap_red_blue(TRUE);
	}
      else
	cairo_riscos_swap_red_blue(TRUE);*/
    }

  _gdk_riscos_display_generate_transtable (riscos_display);
  _gdk_riscos_display_calculate_render_scale (riscos_display);

  g_signal_emit_by_name (_gdk_display, "opened");
  g_signal_emit_by_name (gdk_display_manager_get (), "display-opened", _gdk_display);

  return _gdk_display;
}

static const gchar *
gdk_riscos_display_get_name (GdkDisplay *display)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  return (gchar *) "RISC OS";
}

static GdkScreen *
gdk_riscos_display_get_default_screen (GdkDisplay *display)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  return GDK_RISCOS_DISPLAY (display)->default_screen;
}

static void
gdk_riscos_display_beep (GdkDisplay *display)
{
  g_return_if_fail (GDK_IS_DISPLAY (display));
}

static void
gdk_riscos_display_sync (GdkDisplay *display)
{
  g_return_if_fail (GDK_IS_DISPLAY (display));

}

static void
gdk_riscos_display_flush (GdkDisplay *display)
{
}

static gboolean
gdk_riscos_display_has_pending (GdkDisplay *display)
{
  return FALSE;
}

static GdkWindow *
gdk_riscos_display_get_default_group (GdkDisplay *display)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  return NULL;
}

static void
gdk_riscos_display_dispose (GObject *object)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (object);

  _gdk_riscos_display_manager_remove_display (gdk_display_manager_get (),
						GDK_DISPLAY_OBJECT (object));

  _gdk_screen_close (riscos_display->screens[0]);

  G_OBJECT_CLASS (gdk_riscos_display_parent_class)->dispose (object);
}

static void
gdk_riscos_display_finalize (GObject *object)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (object);

  /* Keymap */
  if (riscos_display->keymap)
    g_object_unref (riscos_display->keymap);

#if 0
  _gdk_riscos_cursor_display_finalize (GDK_DISPLAY_OBJECT(riscos_display));

  /* input GdkDevice list */
  g_list_free_full (riscos_display->input_devices, g_object_unref);
#endif

  /* Free all GdkScreens */
  g_object_unref (riscos_display->screens[0]);
  g_free (riscos_display->screens);

  G_OBJECT_CLASS (gdk_riscos_display_parent_class)->finalize (object);
}

static void
gdk_riscos_display_notify_startup_complete (GdkDisplay  *display,
					      const gchar *startup_id)
{
}

static gboolean
gdk_riscos_display_supports_selection_notification (GdkDisplay *display)
{
  return FALSE;
}

static gboolean
gdk_riscos_display_request_selection_notification (GdkDisplay *display,
						     GdkAtom     selection)

{
    return FALSE;
}

static gboolean
gdk_riscos_display_supports_clipboard_persistence (GdkDisplay *display)
{
  return FALSE;
}

static void
gdk_riscos_display_store_clipboard (GdkDisplay    *display,
				      GdkWindow     *clipboard_window,
				      guint32        time_,
				      const GdkAtom *targets,
				      gint           n_targets)
{
}

static gboolean
gdk_riscos_display_supports_shapes (GdkDisplay *display)
{
  return FALSE;
}

static gboolean
gdk_riscos_display_supports_input_shapes (GdkDisplay *display)
{
  return FALSE;
}

static gboolean
gdk_riscos_display_supports_composite (GdkDisplay *display)
{
  return FALSE;
}

static gulong
gdk_riscos_display_get_next_serial (GdkDisplay *display)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  guint next_serial = riscos_display->next_serial++;

  return next_serial;
}

static void
gdk_riscos_display_event_data_copy (GdkDisplay    *display,
				      const GdkEvent *src,
				      GdkEvent       *dst)
{
  /* Nothing.  */
}

static void
gdk_riscos_display_event_data_free (GdkDisplay    *display,
				      GdkEvent *event)
{
  /* Nothing.  */
}

static void
_gdk_riscos_display_before_process_all_updates (GdkDisplay *display)
{
}

static void
_gdk_riscos_display_after_process_all_updates (GdkDisplay *display)
{
}

/**
 * gdk_riscos_window_lookup_for_display:
 * @display: (type GdkRiscosDisplay): the #GdkDisplay corresponding to the
 *           window handle
 * @window_handle: a RISC OS <type>wimp_w</type>
 *
 * Looks up the #GdkWindow that wraps the given native window handle.
 *
 * Return value: (transfer none): the #GdkWindow wrapper for the native
 *    window, or %NULL if there is none.
 *
 */
GdkWindow *
gdk_riscos_window_lookup_for_display (GdkDisplay *display,
				      wimp_w window_handle)
{
  GdkRiscosDisplay *display_riscos;
  GdkWindow *data = NULL;

  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  display_riscos = GDK_RISCOS_DISPLAY (display);

  if (display_riscos->id_ht)
    data = g_hash_table_lookup (display_riscos->id_ht, window_handle);

  return data;
}

guint
gdk_riscos_display_get_user_time (GdkDisplay *display)
{
  GdkRiscosDisplay *display_riscos;

  display_riscos = GDK_RISCOS_DISPLAY (display);

  return display_riscos->last_seen_time;
}

void
_gdk_riscos_display_set_last_seen_time (GdkRiscosDisplay *display)
{
  display->last_seen_time = os_read_monotonic_time() * 10;
}

void gdk_riscos_enable_toolbox (const char *resource_dir_name,
				toolbox_block *tb_block,
				messagetrans_control_block *tb_messages)
{
  /* Unfortunately, at this point the display doesn't exist yet, so we'll
   * have to stash these in globals until later.  */
  _tb_block = tb_block;
  _tb_messages = tb_messages;
  _resource_dir_name = resource_dir_name;
}

void _gdk_riscos_display_generate_transtable(GdkRiscosDisplay *rdisplay)
{
  if (rdisplay->trans_table != NULL)
    {
      free(rdisplay->trans_table);
      rdisplay->trans_table = NULL;
    }

  int size;
  os_error *err;

  /* Assume we always render to a 32bit colour sprite (at least I've
   * never seen any other type used yet).  */
  err = xcolourtrans_generate_table((os_mode)(1 | (6 << 27) | (90 << 1) | (90 << 14)),
				    0,
				    os_CURRENT_MODE,
				    colourtrans_CURRENT_PALETTE,
				    NULL,
				    0,
				    NULL,
				    NULL,
				    &size);
  if (err)
    {
      g_warning("RISC OS: Failed to find size of pixel translation table");
      return;
    }

  if (size == 0)
    return;

  rdisplay->trans_table = malloc(size);
  if (!rdisplay->trans_table)
    {
      g_warning("RISC OS: Failed to allocate memory for pixel translation table");
      return;
    }

  err = xcolourtrans_generate_table((os_mode)(1 | (6 << 27) | (90 << 1) | (90 << 14)),
				    0,
				    os_CURRENT_MODE,
				    colourtrans_CURRENT_PALETTE,
				    rdisplay->trans_table,
				    0,
				    NULL,
				    NULL,
				    NULL);
  if (err)
    {
      g_warning("RISC OS: Failed to generate pixel translation table");
      return;
    }
}

void _gdk_riscos_display_calculate_render_scale (GdkRiscosDisplay *rdisplay)
{
#define XEIG_FACTOR 0
#define YEIG_FACTOR 1

  os_VDU_VAR_LIST(3) var_list = { { os_VDUVAR_XEIG_FACTOR,
				  os_VDUVAR_YEIG_FACTOR,
				  os_VDUVAR_END_LIST } };

  xos_read_vdu_variables ((os_vdu_var_list *)&var_list, (int *)&var_list);

  if (var_list.var[XEIG_FACTOR] == 0)
    {
      rdisplay->render_scale.xmul = 2;
      rdisplay->render_scale.xdiv = 1;
    }
  else
    {
      rdisplay->render_scale.xmul = 1;
      rdisplay->render_scale.xdiv = var_list.var[XEIG_FACTOR];
    }

  if (var_list.var[YEIG_FACTOR] == 0)
    {
      rdisplay->render_scale.ymul = 2;
      rdisplay->render_scale.ydiv = 1;
    }
  else
    {
      rdisplay->render_scale.ymul = 1;
      rdisplay->render_scale.ydiv = var_list.var[YEIG_FACTOR];
    }

  if (rdisplay->render_scale.xmul == 1 && rdisplay->render_scale.xdiv == 1 &&
      rdisplay->render_scale.ymul == 1 && rdisplay->render_scale.ydiv == 1)
    rdisplay->render_scale_ptr = NULL;
  else
    rdisplay->render_scale_ptr = &rdisplay->render_scale;

#undef XEIG_FACTOR
#undef YEIG_FACTOR
}

static int
gdk_riscos_display_get_n_monitors (GdkDisplay *display)
{
  return 1;
}

static GdkMonitor *
gdk_riscos_display_get_monitor (GdkDisplay *display,
				int monitor_num)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  if (monitor_num == 0)
    return riscos_display->monitor;

  return NULL;
}

static GdkMonitor *
gdk_riscos_display_get_primary_monitor (GdkDisplay *display)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  return riscos_display->monitor;
}

static void
gdk_riscos_display_class_init (GdkRiscosDisplayClass * class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GdkDisplayClass *display_class = GDK_DISPLAY_CLASS (class);

  object_class->dispose = gdk_riscos_display_dispose;
  object_class->finalize = gdk_riscos_display_finalize;

  display_class->window_type = GDK_TYPE_RISCOS_WINDOW;

  display_class->get_name = gdk_riscos_display_get_name;
  display_class->get_default_screen = gdk_riscos_display_get_default_screen;
  display_class->beep = gdk_riscos_display_beep;
  display_class->sync = gdk_riscos_display_sync;
  display_class->flush = gdk_riscos_display_flush;
  display_class->has_pending = gdk_riscos_display_has_pending;
  display_class->queue_events = _gdk_riscos_display_queue_events;
  display_class->get_default_group = gdk_riscos_display_get_default_group;
  display_class->supports_selection_notification = gdk_riscos_display_supports_selection_notification;
  display_class->request_selection_notification = gdk_riscos_display_request_selection_notification;
  display_class->supports_clipboard_persistence = gdk_riscos_display_supports_clipboard_persistence;
  display_class->store_clipboard = gdk_riscos_display_store_clipboard;
  display_class->supports_shapes = gdk_riscos_display_supports_shapes;
  display_class->supports_input_shapes = gdk_riscos_display_supports_input_shapes;
  display_class->supports_composite = gdk_riscos_display_supports_composite;
  display_class->get_cursor_for_type = _gdk_riscos_display_get_cursor_for_type;
  display_class->get_cursor_for_name = _gdk_riscos_display_get_cursor_for_name;
  display_class->get_default_cursor_size = _gdk_riscos_display_get_default_cursor_size;
  display_class->get_maximal_cursor_size = _gdk_riscos_display_get_maximal_cursor_size;
  display_class->supports_cursor_alpha = _gdk_riscos_display_supports_cursor_alpha;
  display_class->supports_cursor_color = _gdk_riscos_display_supports_cursor_color;

  display_class->before_process_all_updates = _gdk_riscos_display_before_process_all_updates;
  display_class->after_process_all_updates = _gdk_riscos_display_after_process_all_updates;
  display_class->get_next_serial = gdk_riscos_display_get_next_serial;
  display_class->notify_startup_complete = gdk_riscos_display_notify_startup_complete;
  display_class->event_data_copy = gdk_riscos_display_event_data_copy;
  display_class->event_data_free = gdk_riscos_display_event_data_free;
  display_class->create_window_impl = _gdk_riscos_display_create_window_impl;
  display_class->get_keymap = _gdk_riscos_display_get_keymap;
  display_class->get_selection_owner = _gdk_riscos_display_get_selection_owner;
  display_class->set_selection_owner = _gdk_riscos_display_set_selection_owner;
  display_class->send_selection_notify = _gdk_riscos_display_send_selection_notify;
  display_class->get_selection_property = _gdk_riscos_display_get_selection_property;
  display_class->convert_selection = _gdk_riscos_display_convert_selection;
  display_class->text_property_to_utf8_list = _gdk_riscos_display_text_property_to_utf8_list;
  display_class->utf8_to_string_target = _gdk_riscos_display_utf8_to_string_target;

  display_class->get_n_monitors = gdk_riscos_display_get_n_monitors;
  display_class->get_monitor = gdk_riscos_display_get_monitor;
  display_class->get_primary_monitor = gdk_riscos_display_get_primary_monitor;
}
