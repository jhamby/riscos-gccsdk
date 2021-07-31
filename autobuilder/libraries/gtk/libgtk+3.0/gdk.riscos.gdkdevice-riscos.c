/* GDK - The GIMP Drawing Kit
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Written by Lee Noar using code and guidance from other targets.
 */

#include "config.h"
#include <stdlib.h>

#include "gdkdevice-riscos.h"
#include "gdkscreen-riscos.h"
#include "gdkwindow.h"
#include "gdkprivate-riscos.h"

#include "oslib/wimp.h"
#include "oslib/report.h"

static gboolean gdk_riscos_device_get_history (GdkDevice      *device,
						 GdkWindow      *window,
						 guint32         start,
						 guint32         stop,
						 GdkTimeCoord ***events,
						 gint           *n_events);
static void gdk_riscos_device_get_state (GdkDevice       *device,
					   GdkWindow       *window,
					   gdouble         *axes,
					   GdkModifierType *mask);
static void gdk_riscos_device_set_window_cursor (GdkDevice *device,
						   GdkWindow *window,
						   GdkCursor *cursor);
static void gdk_riscos_device_warp (GdkDevice   *device,
				    GdkScreen *screen,
				    gint    x,
				    gint    y);
static void gdk_riscos_device_query_state (GdkDevice        *device,
                                             GdkWindow        *window,
                                             GdkWindow       **root_window,
                                             GdkWindow       **child_window,
                                             gint          *root_x,
                                             gint          *root_y,
                                             gint          *win_x,
                                             gint          *win_y,
                                             GdkModifierType  *mask);
static GdkGrabStatus gdk_riscos_device_grab   (GdkDevice     *device,
						 GdkWindow     *window,
						 gboolean       owner_events,
						 GdkEventMask   event_mask,
						 GdkWindow     *confine_to,
						 GdkCursor     *cursor,
						 guint32        time_);
static void          gdk_riscos_device_ungrab (GdkDevice     *device,
						 guint32        time_);
static GdkWindow * gdk_riscos_device_window_at_position (GdkDevice       *device,
							   gint       *win_x,
							   gint       *win_y,
							   GdkModifierType *mask,
							   gboolean         get_toplevel);
static void      gdk_riscos_device_select_window_events (GdkDevice       *device,
							   GdkWindow       *window,
							   GdkEventMask     event_mask);


G_DEFINE_TYPE (GdkRiscosDevice, gdk_riscos_device, GDK_TYPE_DEVICE)

static void
gdk_riscos_device_class_init (GdkRiscosDeviceClass *klass)
{
  GdkDeviceClass *device_class = GDK_DEVICE_CLASS (klass);

  device_class->get_history = gdk_riscos_device_get_history;
  device_class->get_state = gdk_riscos_device_get_state;
  device_class->set_window_cursor = gdk_riscos_device_set_window_cursor;
  device_class->warp = gdk_riscos_device_warp;
  device_class->query_state = gdk_riscos_device_query_state;
  device_class->grab = gdk_riscos_device_grab;
  device_class->ungrab = gdk_riscos_device_ungrab;
  device_class->window_at_position = gdk_riscos_device_window_at_position;
  device_class->select_window_events = gdk_riscos_device_select_window_events;
}

static void
gdk_riscos_device_init (GdkRiscosDevice *device_core)
{
  GdkDevice *device;

  device = GDK_DEVICE (device_core);

  _gdk_device_add_axis (device, GDK_NONE, GDK_AXIS_X, 0, 0, 1);
  _gdk_device_add_axis (device, GDK_NONE, GDK_AXIS_Y, 0, 0, 1);
}

static gboolean
gdk_riscos_device_get_history (GdkDevice      *device,
				 GdkWindow      *window,
				 guint32         start,
				 guint32         stop,
				 GdkTimeCoord ***events,
				 gint           *n_events)
{
  return FALSE;
}

static void
gdk_riscos_device_get_state (GdkDevice       *device,
			       GdkWindow       *window,
			       gdouble         *axes,
			       GdkModifierType *mask)
{
  gint x_int, y_int;

  gdk_window_get_pointer (window, &x_int, &y_int, mask);

  if (axes)
    {
      axes[0] = x_int;
      axes[1] = y_int;
    }
}

static void
gdk_riscos_device_set_window_cursor (GdkDevice *device,
				     GdkWindow *window,
				     GdkCursor *cursor)
{
}

static void
gdk_riscos_device_warp (GdkDevice   *device,
			GdkScreen   *screen,
			gint      x,
			gint      y)
{
}

static void
gdk_riscos_device_query_state (GdkDevice        *device,
				GdkWindow        *window,
				GdkWindow       **root_window,
				GdkWindow       **child_window,
				gint          *root_x,
				gint          *root_y,
				gint          *win_x,
				gint          *win_y,
				GdkModifierType  *mask)
{
  GdkWindow *toplevel;
  GdkWindowImplRiscos *impl;
  GdkDisplay *display;
  GdkRiscosDisplay *riscos_display;
  GdkWindow *mouse_toplevel;
  
  wimp_pointer mouse;
  
  if (gdk_device_get_source (device) != GDK_SOURCE_MOUSE)
    return;

  display = gdk_device_get_display (device);
  riscos_display = GDK_RISCOS_DISPLAY (display);

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
  toplevel = impl->wrapper;

  if (root_window)
    {
      GdkRiscosScreen *riscos_screen = (GdkRiscosScreen *)gdk_window_get_screen (window);
      *root_window = gdk_screen_get_root_window (&riscos_screen->parent_instance);
    }

  xwimp_get_pointer_info (&mouse);

  mouse_toplevel = g_hash_table_lookup (riscos_display->id_ht, GUINT_TO_POINTER (mouse.w));

  GdkPoint mouse_pos;
  gdk_riscos_screen_point_to_pixel (impl->screen, (GdkPoint *)&mouse.pos, &mouse_pos);

  /* Bitwise OR to see if either of these are non zero.  */
  if ((guint)root_x | (guint)root_y)
    {
      GdkPoint global_pos;

      gdk_riscos_screen_global_point (impl->screen, &mouse_pos, &global_pos);

      if (root_x)
	*root_x = global_pos.x;
      if (root_y)
	*root_y = global_pos.y;
    }

  /* Bitwise OR to see if either of these are non zero.  */
  if (((guint)win_x | (guint)win_y) && mouse_toplevel)
    {
      GdkWindowImplRiscos *mouse_toplevel_impl = GDK_WINDOW_IMPL_RISCOS (mouse_toplevel->impl);
      GdkPoint local_pos;

      gdk_riscos_window_map_from_global (mouse_toplevel_impl->wrapper, &mouse_pos, &local_pos);

      if (win_x)
	*win_x = local_pos.x;
      if (win_y)
	*win_y = local_pos.y;
    }

  if (mask)
    {
      guint modifiers = gdk_riscos_read_modifiers ();
      guint buttons = gdk_riscos_translate_buttons (mouse.buttons);

      *mask = modifiers | buttons;
    }

  if (child_window)
    {
      if (gdk_window_get_window_type (toplevel) == GDK_WINDOW_ROOT)
	{
	  *child_window = mouse_toplevel;
	  if (*child_window == NULL)
	    *child_window = toplevel;
	}
      else
	{
	  /* No native children */
	  *child_window = toplevel;
	}
    }

  return;
}

void
_gdk_riscos_window_grab_check_unmap (GdkWindow *window,
				       gulong     serial)
{
  GdkDisplay *display = gdk_window_get_display (window);
  GdkDeviceManager *device_manager;
  GList *devices, *d;

  device_manager = gdk_display_get_device_manager (display);

  /* Get all devices */
  devices = gdk_device_manager_list_devices (device_manager, GDK_DEVICE_TYPE_MASTER);
  devices = g_list_concat (devices, gdk_device_manager_list_devices (device_manager, GDK_DEVICE_TYPE_SLAVE));
  devices = g_list_concat (devices, gdk_device_manager_list_devices (device_manager, GDK_DEVICE_TYPE_FLOATING));

  /* End all grabs on the newly hidden window */
  for (d = devices; d; d = d->next)
    _gdk_display_end_device_grab (display, d->data, serial, window, TRUE);

  g_list_free (devices);
}


void
_gdk_riscos_window_grab_check_destroy (GdkWindow *window)
{
  GdkDisplay *display = gdk_window_get_display (window);
  GdkDeviceManager *device_manager;
  GdkDeviceGrabInfo *grab;
  GList *devices, *d;

  device_manager = gdk_display_get_device_manager (display);

  /* Get all devices */
  devices = gdk_device_manager_list_devices (device_manager, GDK_DEVICE_TYPE_MASTER);

  for (d = devices; d; d = d->next)
    {
      /* Make sure there is no lasting grab in this native window */
      grab = _gdk_display_get_last_device_grab (display, d->data);

      if (grab && grab->native_window == window)
	{
	  grab->serial_end = grab->serial_start;
	  grab->implicit_ungrab = TRUE;
	}

    }

  g_list_free (devices);
}


static GdkGrabStatus
gdk_riscos_device_grab (GdkDevice    *device,
			GdkWindow    *window,
			gboolean      owner_events,
			GdkEventMask  event_mask,
			GdkWindow    *confine_to,
			GdkCursor    *cursor,
			guint32       time_)
{
#if 1
  return GDK_GRAB_SUCCESS;
#else
  GdkDisplay *display;
  GdkRiscosDisplay *riscos_display;

  display = gdk_device_get_display (device);
  riscos_display = GDK_RISCOS_DISPLAY (display);

  if (gdk_device_get_source (device) == GDK_SOURCE_KEYBOARD)
    {
      /* Device is a keyboard */
      return GDK_GRAB_SUCCESS;
    }
  else
    {
      /* Device is a pointer */
      {
	char buffer[100];
	
	sprintf (buffer, "\\r%s",__func__);
	report_text0 (buffer);
      }

      if (riscos_display->pointer_grab.window != NULL)
	return GDK_GRAB_ALREADY_GRABBED;

      riscos_display->pointer_grab.window = window;

      return GDK_GRAB_SUCCESS;
    }
#endif
}

#define TIME_IS_LATER(time1, time2)                        \
  ( (( time1 > time2 ) && ( time1 - time2 < ((guint32)-1)/2 )) ||  \
    (( time1 < time2 ) && ( time2 - time1 > ((guint32)-1)/2 ))     \
  )

static void
gdk_riscos_device_ungrab (GdkDevice *device,
			  guint32    time_)
{
#if 1
  GdkDeviceGrabInfo *grab;

  grab = _gdk_display_get_last_device_grab (_gdk_display, device);
  if (grab)
    grab->serial_end = 0;

  _gdk_display_device_grab_update (_gdk_display, device, NULL, 0);
#else
  GdkDisplay *display;
  GdkRiscosDisplay *riscos_display;
  GdkDeviceGrabInfo *grab;
  guint32 serial;

  display = gdk_device_get_display (device);
  riscos_display = GDK_RISCOS_DISPLAY (display);

  if (gdk_device_get_source (device) == GDK_SOURCE_KEYBOARD)
    {
      /* Device is a keyboard */
    }
  else
    {
      {
	char buffer[100];
	
	sprintf (buffer, "\\g%s",__func__);
	report_text0 (buffer);
      }
      /* Device is a pointer */
      riscos_display->pointer_grab.window = NULL;
/*      serial = _gdk_riscos_server_ungrab_pointer (riscos_display->server, time_);

      if (serial != 0)
	{
	  grab = _gdk_display_get_last_device_grab (display, device);
	  if (grab &&
	      (time_ == GDK_CURRENT_TIME ||
	       grab->time == GDK_CURRENT_TIME ||
	       !TIME_IS_LATER (grab->time, time_)))
	    grab->serial_end = serial;
	}*/
    }
#endif
}

static void
translate_coords_to_child_coords (GdkWindow *parent,
                                  GdkWindow *child,
                                  gint      *x,
                                  gint      *y)
{
  GdkWindow *current = child;

  if (child == parent)
    return;

  while (current != parent && gdk_window_get_window_type (current) != GDK_WINDOW_TOPLEVEL)
    {
      gint tmp_x, tmp_y;

      gdk_window_get_origin (current, &tmp_x, &tmp_y);

      *x -= tmp_x;
      *y -= tmp_y;

      current = gdk_window_get_effective_parent (current);
    }
}

static GdkWindow *
gdk_riscos_device_window_at_position (GdkDevice       *device,
				      gint            *win_x,
				      gint            *win_y,
				      GdkModifierType *mask,
				      gboolean         get_toplevel)
{
  GdkScreen *screen;
  GdkWindow *root_window;
  GdkWindow *found_window;
  gint x_tmp, y_tmp;
  
  screen = gdk_screen_get_default ();
  root_window = gdk_screen_get_root_window (screen);
  
  gdk_riscos_device_query_state (device, root_window, NULL, &found_window, NULL, NULL, &x_tmp, &y_tmp, mask);

  found_window = _gdk_riscos_window_find_child (found_window, x_tmp, y_tmp, get_toplevel);

  if (found_window)
    translate_coords_to_child_coords (root_window, found_window,
				      &x_tmp, &y_tmp);

  if (win_x)
    *win_x = found_window ? x_tmp : -1;

  if (win_y)
    *win_y = found_window ? y_tmp : -1;

  return found_window;
}

static void
gdk_riscos_device_select_window_events (GdkDevice    *device,
					GdkWindow    *window,
					GdkEventMask  event_mask)
{
  /* The mask is set in the common code. */
}
