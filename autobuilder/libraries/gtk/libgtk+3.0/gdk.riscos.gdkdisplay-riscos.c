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
#include "gdkwindow-riscos.h"
#include "gdkprivate-riscos.h"

#include "cairo-riscos.h"

#include "oslib/os.h"

#include <string.h>

static void   gdk_riscos_display_dispose            (GObject            *object);
static void   gdk_riscos_display_finalize           (GObject            *object);

G_DEFINE_TYPE (GdkRiscosDisplay, gdk_riscos_display, GDK_TYPE_DISPLAY)

static void
gdk_riscos_display_init (GdkRiscosDisplay *display)
{
  _gdk_riscos_display_manager_add_display (gdk_display_manager_get (),
					   GDK_DISPLAY_OBJECT (display));
  display->id_ht = g_hash_table_new (NULL, NULL);

  display->pointer_grab.window = NULL;
  display->keyboard_grab.window = NULL;

  display->system_drag = FALSE;
  display->last_mouse_move.x = -1;
  display->last_mouse_move.y = -1;
  display->last_click.w = 0;

  display->key_down_list = NULL;

  display->raw_event_handlers = NULL;

  display->focus_window = NULL;
}

static void
gdk_event_init (GdkDisplay *display)
{
  GdkRiscosDisplay *riscos_display;

  riscos_display = GDK_RISCOS_DISPLAY (display);
//  riscos_display->event_source = _gdk_riscos_event_source_new (display);
  riscos_display->next_serial = 1;
  riscos_display->last_seen_time = 0;
  _gdk_riscos_events_init (display);
}

static os_error *
gdk_riscos_create_iconbar_icon (GdkRiscosDisplay *display)
{
  wimp_icon_create bar;

  bar.w = wimp_ICON_BAR_RIGHT;
  bar.icon.extent.x0 = 0;
  bar.icon.extent.y0 = 0;
  bar.icon.extent.x1 = 68;
  bar.icon.extent.y1 = 68;
  bar.icon.flags = wimp_ICON_SPRITE |
		   wimp_ICON_HCENTRED |
		   wimp_ICON_VCENTRED |
		   wimp_ICON_SPRITE |
		   wimp_ICON_HCENTRED |
		   wimp_ICON_VCENTRED |
		   (wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT);
  if (*display->task_name != '!') {
    *bar.icon.data.sprite = '!';
    memcpy (bar.icon.data.sprite + 1, display->task_name, 11);
  } else
    memcpy (bar.icon.data.sprite, display->task_name, 12);
  
  return xwimp_create_icon (&bar, &display->iconbar_icon_handle);
}

static void
gdk_riscos_wimp_init (GdkRiscosDisplay *display)
{
  wimp_MESSAGE_LIST(2) message_list = { { 0 } };

  os_error *err;

  display->task_name = g_get_prgname ();
  if (display->task_name == NULL)
    g_error ("Could not determine name of task to initialise") ;
    
  err = xwimp_initialise (wimp_VERSION_RO38,
			  display->task_name,
			  (wimp_message_list *)&message_list,
			  NULL,
			  &display->task_handle);
  if (err != NULL)
    g_error ("Failed to initialise task; %s", err->errmess);

  err = gdk_riscos_create_iconbar_icon (display);
  if (err != NULL)
    g_error ("Failed to create iconbar icon");

/*  display->iconbar = gdk_riscos_window_foreign_new_for_display (&display->parent_instance,
								wimp_ICON_BAR);*/
}

static void
gdk_riscos_display_init_input (GdkDisplay *display)
{
  GdkRISCOSDeviceManager *device_manager;

  device_manager = (GdkRISCOSDeviceManager *)gdk_display_get_device_manager (display);
  display->core_pointer = device_manager->core_pointer;
}

GdkDisplay *
_gdk_riscos_display_open (const gchar *display_name)
{
  GdkRiscosDisplay *riscos_display;
//  GError *error;

  _gdk_display = g_object_new (GDK_TYPE_RISCOS_DISPLAY, NULL);
  riscos_display = GDK_RISCOS_DISPLAY (_gdk_display);

  gdk_riscos_wimp_init (riscos_display);
//  riscos_display->output = NULL;

  /* Initialize the display's screens */
  riscos_display->screens = g_new (GdkScreen *, 1);
  riscos_display->screens[0] = _gdk_riscos_screen_new (_gdk_display, 0);

  /* We need to initialize events after we have the screen
   * structures in places
   */
//  _gdk_riscos_screen_events_init (riscos_display->screens[0]);

  /*set the default screen */
  riscos_display->default_screen = riscos_display->screens[0];
  riscos_display->keymap = NULL;

  _gdk_display->device_manager = _gdk_riscos_device_manager_new (_gdk_display);

  gdk_event_init (_gdk_display);

  gdk_riscos_display_init_input (_gdk_display);

#if 0
  _gdk_riscos_display_init_dnd (_gdk_display);
#endif

  _gdk_riscos_screen_setup (riscos_display->screens[0]);

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

static gint
gdk_riscos_display_get_n_screens (GdkDisplay *display)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), 0);

  return 1;
}

static GdkScreen *
gdk_riscos_display_get_screen (GdkDisplay *display,
				 gint        screen_num)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);
  g_return_val_if_fail (screen_num == 0, NULL);

  return GDK_RISCOS_DISPLAY (display)->screens[screen_num];
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
//  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  g_return_if_fail (GDK_IS_DISPLAY (display));
#if 0
  if (riscos_display->output &&
      !broadway_output_flush (riscos_display->output))
    {
      riscos_display->saved_serial = broadway_output_get_next_serial (riscos_display->output);
      broadway_output_free (riscos_display->output);
      riscos_display->output = NULL;
    }
#endif
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

//  g_list_foreach (riscos_display->input_devices, (GFunc) g_object_run_dispose, NULL);

  _gdk_screen_close (riscos_display->screens[0]);
#if 0
  if (riscos_display->event_source)
    {
      g_source_destroy (riscos_display->event_source);
      g_source_unref (riscos_display->event_source);
      riscos_display->event_source = NULL;
    }
#endif
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

static GList *
gdk_riscos_display_list_devices (GdkDisplay *display)
{
  return NULL;
#if 0
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  return GDK_RISCOS_DISPLAY (display)->input_devices;
#endif
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

static void
gdk_riscos_display_class_init (GdkRiscosDisplayClass * class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GdkDisplayClass *display_class = GDK_DISPLAY_CLASS (class);

  object_class->dispose = gdk_riscos_display_dispose;
  object_class->finalize = gdk_riscos_display_finalize;

  display_class->window_type = GDK_TYPE_RISCOS_WINDOW;

  display_class->get_name = gdk_riscos_display_get_name;
  display_class->get_n_screens = gdk_riscos_display_get_n_screens;
  display_class->get_screen = gdk_riscos_display_get_screen;
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
  display_class->list_devices = gdk_riscos_display_list_devices;
  display_class->get_cursor_for_type = _gdk_riscos_display_get_cursor_for_type;
  display_class->get_cursor_for_name = _gdk_riscos_display_get_cursor_for_name;
  display_class->get_cursor_for_pixbuf = _gdk_riscos_display_get_cursor_for_pixbuf;
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
}
