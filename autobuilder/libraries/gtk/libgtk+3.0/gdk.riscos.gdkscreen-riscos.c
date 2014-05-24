 /*
 * gdkscreen-riscos.c
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

#include "gdkprivate-riscos.h"
#include "gdkscreen-riscos.h"
#include "gdkscreen.h"
#include "gdkdisplay.h"
#include "gdkdisplay-riscos.h"

#include "oslib/os.h"

#include <glib.h>

#include <stdlib.h>
#include <string.h>

static void   gdk_riscos_screen_dispose     (GObject *object);
static void   gdk_riscos_screen_finalize    (GObject *object);

G_DEFINE_TYPE (GdkRiscosScreen, gdk_riscos_screen, GDK_TYPE_SCREEN)

static void
gdk_riscos_screen_init (GdkRiscosScreen *screen)
{
  os_VDU_VAR_LIST(5) var_list = { { os_VDUVAR_XEIG_FACTOR,
				  os_VDUVAR_YEIG_FACTOR,
				  os_VDUVAR_XWIND_LIMIT,
				  os_VDUVAR_YWIND_LIMIT,
				  os_VDUVAR_END_LIST } };

  xos_read_vdu_variables ((os_vdu_var_list *)&var_list, (int *)&var_list);

  screen->x_eigen_factor = var_list.var[0];
  screen->y_eigen_factor = var_list.var[1];
  screen->width = var_list.var[2] + 1;
  screen->height = var_list.var[3] + 1;
}

static GdkDisplay *
gdk_riscos_screen_get_display (GdkScreen *screen)
{
  return GDK_RISCOS_SCREEN (screen)->display;
}

static gint
gdk_riscos_screen_get_width (GdkScreen *screen)
{
  return GDK_RISCOS_SCREEN (screen)->width;
}

static gint
gdk_riscos_screen_get_height (GdkScreen *screen)
{
  return GDK_RISCOS_SCREEN (screen)->height;
}

static gint
gdk_riscos_screen_get_width_mm (GdkScreen *screen)
{
  return gdk_screen_get_width (screen) * 25.4 / 96;
}

static gint
gdk_riscos_screen_get_height_mm (GdkScreen *screen)
{
  return gdk_screen_get_height (screen) * 25.4 / 96;
}

static gint
gdk_riscos_screen_get_number (GdkScreen *screen)
{
  return 0;
}

static GdkWindow *
gdk_riscos_screen_get_root_window (GdkScreen *screen)
{
  return GDK_RISCOS_SCREEN (screen)->root_window;
}

/*void
_gdk_riscos_screen_size_changed (GdkScreen                       *screen,
                                   BroadwayInputScreenResizeNotify *msg)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);
  gint width, height;

  width = gdk_screen_get_width (screen);
  height = gdk_screen_get_height (screen);

  riscos_screen->width   = msg->width;
  riscos_screen->height  = msg->height;

  if (width != gdk_screen_get_width (screen) ||
      height != gdk_screen_get_height (screen))
    g_signal_emit_by_name (screen, "size-changed");
}*/

static void
gdk_riscos_screen_dispose (GObject *object)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (object);

  if (riscos_screen->root_window)
    _gdk_window_destroy (riscos_screen->root_window, TRUE);

  G_OBJECT_CLASS (gdk_riscos_screen_parent_class)->dispose (object);
}

static void
gdk_riscos_screen_finalize (GObject *object)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (object);
  gint          i;

  if (riscos_screen->root_window)
    g_object_unref (riscos_screen->root_window);

  /* Visual Part */
  for (i = 0; i < riscos_screen->nvisuals; i++)
    g_object_unref (riscos_screen->visuals[i]);
  g_free (riscos_screen->visuals);

  G_OBJECT_CLASS (gdk_riscos_screen_parent_class)->finalize (object);
}

static gint
gdk_riscos_screen_get_n_monitors (GdkScreen *screen)
{
  return 1;
}

static gint
gdk_riscos_screen_get_primary_monitor (GdkScreen *screen)
{
  return 0;
}

static gint
gdk_riscos_screen_get_monitor_width_mm (GdkScreen *screen,
					  gint       monitor_num)
{
  return gdk_screen_get_width_mm (screen);
}

static gint
gdk_riscos_screen_get_monitor_height_mm (GdkScreen *screen,
					   gint       monitor_num)
{
  return gdk_screen_get_height_mm (screen);
}

static gchar *
gdk_riscos_screen_get_monitor_plug_name (GdkScreen *screen,
					   gint       monitor_num)
{
  return g_strdup ("browser");
}

static void
gdk_riscos_screen_get_monitor_geometry (GdkScreen    *screen,
					  gint          monitor_num,
					  GdkRectangle *dest)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);

  if (dest)
    {
      dest->x = 0;
      dest->y = 0;
      dest->width = riscos_screen->width;
      dest->height = riscos_screen->height;
    }
}

static GdkVisual *
gdk_riscos_screen_get_rgba_visual (GdkScreen *screen)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);

  return riscos_screen->rgba_visual;
}

GdkScreen *
_gdk_riscos_screen_new (GdkDisplay *display,
			  gint	 screen_number)
{
  GdkRiscosScreen *riscos_screen;

  _gdk_screen = g_object_new (GDK_TYPE_RISCOS_SCREEN, NULL);

  riscos_screen = GDK_RISCOS_SCREEN (_gdk_screen);
  riscos_screen->display = display;
  _gdk_riscos_screen_init_visuals (_gdk_screen);
  _gdk_riscos_screen_init_root_window (_gdk_screen);

  return _gdk_screen;
}

/*
 * It is important that we first request the selection
 * notification, and then setup the initial state of
 * is_composited to avoid a race condition here.
 */
void
_gdk_riscos_screen_setup (GdkScreen *screen)
{
}

static gboolean
gdk_riscos_screen_is_composited (GdkScreen *screen)
{
  return FALSE;
}


static gchar *
gdk_riscos_screen_make_display_name (GdkScreen *screen)
{
  return g_strdup ("browser");
}

static GdkWindow *
gdk_riscos_screen_get_active_window (GdkScreen *screen)
{
  return NULL;
}

static GList *
gdk_riscos_screen_get_window_stack (GdkScreen *screen)
{
  return NULL;
}

static void
gdk_riscos_screen_broadcast_client_message (GdkScreen *screen,
					      GdkEvent  *event)
{
}

static gboolean
gdk_riscos_screen_get_setting (GdkScreen   *screen,
				 const gchar *name,
				 GValue      *value)
{
  return FALSE;
}

void
_gdk_riscos_screen_events_init (GdkScreen *screen)
{
}

static void
gdk_riscos_screen_class_init (GdkRiscosScreenClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GdkScreenClass *screen_class = GDK_SCREEN_CLASS (klass);

  object_class->dispose = gdk_riscos_screen_dispose;
  object_class->finalize = gdk_riscos_screen_finalize;

  screen_class->get_display = gdk_riscos_screen_get_display;
  screen_class->get_width = gdk_riscos_screen_get_width;
  screen_class->get_height = gdk_riscos_screen_get_height;
  screen_class->get_width_mm = gdk_riscos_screen_get_width_mm;
  screen_class->get_height_mm = gdk_riscos_screen_get_height_mm;
  screen_class->get_number = gdk_riscos_screen_get_number;
  screen_class->get_root_window = gdk_riscos_screen_get_root_window;
  screen_class->get_n_monitors = gdk_riscos_screen_get_n_monitors;
  screen_class->get_primary_monitor = gdk_riscos_screen_get_primary_monitor;
  screen_class->get_monitor_width_mm = gdk_riscos_screen_get_monitor_width_mm;
  screen_class->get_monitor_height_mm = gdk_riscos_screen_get_monitor_height_mm;
  screen_class->get_monitor_plug_name = gdk_riscos_screen_get_monitor_plug_name;
  screen_class->get_monitor_geometry = gdk_riscos_screen_get_monitor_geometry;
  screen_class->get_monitor_workarea = gdk_riscos_screen_get_monitor_geometry;
  screen_class->is_composited = gdk_riscos_screen_is_composited;
  screen_class->make_display_name = gdk_riscos_screen_make_display_name;
  screen_class->get_active_window = gdk_riscos_screen_get_active_window;
  screen_class->get_window_stack = gdk_riscos_screen_get_window_stack;
  screen_class->broadcast_client_message = gdk_riscos_screen_broadcast_client_message;
  screen_class->get_setting = gdk_riscos_screen_get_setting;
  screen_class->get_rgba_visual = gdk_riscos_screen_get_rgba_visual;
  screen_class->get_system_visual = _gdk_riscos_screen_get_system_visual;
  screen_class->visual_get_best_depth = _gdk_riscos_screen_visual_get_best_depth;
  screen_class->visual_get_best_type = _gdk_riscos_screen_visual_get_best_type;
  screen_class->visual_get_best = _gdk_riscos_screen_visual_get_best;
  screen_class->visual_get_best_with_depth = _gdk_riscos_screen_visual_get_best_with_depth;
  screen_class->visual_get_best_with_type = _gdk_riscos_screen_visual_get_best_with_type;
  screen_class->visual_get_best_with_both = _gdk_riscos_screen_visual_get_best_with_both;
  screen_class->query_depths = _gdk_riscos_screen_query_depths;
  screen_class->query_visual_types = _gdk_riscos_screen_query_visual_types;
  screen_class->list_visuals = _gdk_riscos_screen_list_visuals;
}

