/* GDK - The GIMP Drawing Kit
 * Copyright (C) 1995-2007 Peter Mattis, Spencer Kimball,
 * Josh MacDonald, Ryan Lortie
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
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

/*
 * Written by Lee Noar using code and guidance from other targets.
 */

/*
 * The RISC OS window has its origin at the top +ve x, -ve y.
 * GDK expects the window origin to be at the top left, both
 * axis positive, so when converting coordinates we have to take this
 * into account.
 */

#include "config.h"

#include "cairo-riscos.h"

#include "gdkwindow-riscos.h"
#include "gdkscreen-riscos.h"

#include "gdkwindow.h"
#include "gdkwindowimpl.h"
#include "gdkdisplay-riscos.h"
#include "gdkprivate-riscos.h"
#include "gdkinternals.h"
#include "gdkdeviceprivate.h"

#include "oslib/colourtrans.h"
#include "oslib/report.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>


#define WINDOW_TITLE_MAX_LEN 100

//#define DEBUG_WINDOW_SIZE_CHANGED
//#define DEBUG_WINDOW_UPDATE

/* Forward declarations */
static void     gdk_window_riscos_set_background     (GdkWindow      *window,
                                                   cairo_pattern_t *pattern);

static void        gdk_window_impl_riscos_finalize   (GObject            *object);

static const cairo_user_data_key_t gdk_riscos_cairo_key;

#define WINDOW_IS_TOPLEVEL_OR_FOREIGN(window) \
  (GDK_WINDOW_TYPE (window) != GDK_WINDOW_CHILD &&   \
   GDK_WINDOW_TYPE (window) != GDK_WINDOW_OFFSCREEN)

#define WINDOW_IS_TOPLEVEL(window)		     \
  (GDK_WINDOW_TYPE (window) != GDK_WINDOW_CHILD &&   \
   GDK_WINDOW_TYPE (window) != GDK_WINDOW_FOREIGN && \
   GDK_WINDOW_TYPE (window) != GDK_WINDOW_OFFSCREEN)

struct _GdkRiscosWindow {
  GdkWindow parent;
};

struct _GdkRiscosWindowClass {
  GdkWindowClass parent_class;
};

G_DEFINE_TYPE (GdkRiscosWindow, gdk_riscos_window, GDK_TYPE_WINDOW)

static void
gdk_riscos_window_class_init (GdkRiscosWindowClass *riscos_window_class)
{
}

static void
gdk_riscos_window_init (GdkRiscosWindow *riscos_window)
{
}

G_DEFINE_TYPE (GdkWindowImplRiscos,
	       gdk_window_impl_riscos,
	       GDK_TYPE_WINDOW_IMPL)

static void
gdk_window_impl_riscos_init (GdkWindowImplRiscos *impl)
{
  impl->toplevel_window_type = -1;
#if 0
  impl->device_cursor = g_hash_table_new_full (NULL, NULL, NULL,
                                               (GDestroyNotify) g_object_unref);
#endif
}

static void
gdk_window_impl_riscos_finalize (GObject *object)
{
  GdkWindow *wrapper;
  GdkWindowImplRiscos *impl;
  GdkRiscosDisplay *riscos_display;

  g_return_if_fail (GDK_IS_WINDOW_IMPL_RISCOS (object));

  impl = GDK_WINDOW_IMPL_RISCOS (object);

  wrapper = impl->wrapper;

  if (riscos_display->pointer_grab.window == wrapper)
    riscos_display->pointer_grab.window = NULL;

  riscos_display = GDK_RISCOS_DISPLAY (gdk_window_get_display (wrapper));
#if 0
  if (riscos_display->mouse_in_toplevel == GDK_WINDOW (wrapper))
    {
      /* TODO: Send leave + enter event, update cursors, etc */
      riscos_display->mouse_in_toplevel = NULL;
    }
#endif
  g_hash_table_remove (riscos_display->id_ht, GINT_TO_POINTER(impl->id));
#if 0
  if (impl->cursor)
    g_object_unref (impl->cursor);

  g_hash_table_destroy (impl->device_cursor);
#endif
  riscos_display->toplevels = g_list_remove (riscos_display->toplevels, impl);

  G_OBJECT_CLASS (gdk_window_impl_riscos_parent_class)->finalize (object);
}

void
_gdk_riscos_screen_init_root_window (GdkScreen * screen)
{
  GdkWindow *window;
  GdkWindowImplRiscos *impl;
  GdkRiscosScreen *riscos_screen;

  riscos_screen = GDK_RISCOS_SCREEN (screen);

  g_assert (riscos_screen->root_window == NULL);

  riscos_screen->root_window = g_object_new (GDK_TYPE_RISCOS_WINDOW, NULL);
  
  window = riscos_screen->root_window;
  window->impl = g_object_new (GDK_TYPE_WINDOW_IMPL_RISCOS, NULL);
  window->impl_window = window;
  window->visual = gdk_screen_get_system_visual (screen);

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  impl->screen = screen;
  impl->wrapper = window;

  window->window_type = GDK_WINDOW_ROOT;
  window->depth = 24;

  window->x = 0;
  window->y = 0;
  window->abs_x = 0;
  window->abs_y = 0;
  window->width = gdk_screen_get_width (screen);
  window->height = gdk_screen_get_height (screen);
  window->viewable = TRUE;

  _gdk_window_update_size (riscos_screen->root_window);

  _gdk_root = window;
}

static void
gdk_riscos_create_window (GdkWindow 	*gdk_window,
			  GdkWindowAttr *attributes,
			  gint           attributes_mask)
{
  GdkWindowImplRiscos *impl;
  os_error *err;
  wimp_window window_blk;

  impl = GDK_WINDOW_IMPL_RISCOS (gdk_window->impl);

  impl->id = wimp_INVALID_WINDOW;
  impl->title = NULL;
  impl->title_validation = NULL;
  impl->title_max_size = 0;

  window_blk.flags = wimp_WINDOW_NEW_FORMAT |
		     wimp_WINDOW_MOVEABLE;

  if (gdk_window->window_type == GDK_WINDOW_TOPLEVEL)
    {
//      if (attributes_mask & GDK_WA_TITLE)
	{
	  impl->title = g_malloc (WINDOW_TITLE_MAX_LEN);
	  impl->title_validation = NULL;
	  impl->title_max_size = WINDOW_TITLE_MAX_LEN;

	  if (impl->title == NULL)
	    {
	      g_warning ("RISCOS: Failed to allocate memory for top level window title");
	      return;
	    }

	  *impl->title = '\0';

	  window_blk.flags |= wimp_WINDOW_TITLE_ICON;
	  window_blk.flags |= wimp_WINDOW_CLOSE_ICON;
	}

      window_blk.title_fg = wimp_COLOUR_BLACK;
      window_blk.extent.x1 = gdk_riscos_screen_os_width (impl->screen);
      window_blk.extent.y0 = -gdk_riscos_screen_os_height (impl->screen);
    }
  else
    {
      window_blk.title_fg = 0xff; // No frame on child windows.
      window_blk.extent.x1 = gdk_riscos_window_os_width (gdk_window);
      window_blk.extent.y0 = -gdk_riscos_window_os_height (gdk_window);
    }

  window_blk.title_bg = wimp_COLOUR_LIGHT_GREY;
  window_blk.work_fg = wimp_COLOUR_BLACK;
  window_blk.work_bg = wimp_COLOUR_WHITE;
  window_blk.scroll_outer = wimp_COLOUR_MID_LIGHT_GREY;
  window_blk.scroll_inner = wimp_COLOUR_VERY_LIGHT_GREY;
  window_blk.highlight_bg = wimp_COLOUR_CREAM;
  window_blk.extra_flags = 0;
  window_blk.extent.x0 = 0;
  window_blk.extent.y1 = 0;
  window_blk.title_flags = wimp_ICON_TEXT |
		       wimp_ICON_HCENTRED |
		       wimp_ICON_VCENTRED |
		       wimp_ICON_INDIRECTED;
  window_blk.work_flags = wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT;
  window_blk.xmin = 0;
  window_blk.ymin = 0;
  window_blk.icon_count = 0;
  
  window_blk.title_data.indirected_text_and_sprite.text = impl->title;
  window_blk.title_data.indirected_text_and_sprite.validation = (char *)-1;
  window_blk.title_data.indirected_text_and_sprite.size = WINDOW_TITLE_MAX_LEN;

  err = xwimp_create_window (&window_blk, &impl->id);
  if (err != NULL)
    g_warning ("RISCOS: Failed to create window; %s", err->errmess);
}

static const gchar *
get_default_title (void)
{
  const char *title;

  title = g_get_application_name ();
  if (!title)
    title = g_get_prgname ();
  if (!title)
    title = "";

  return title;
}

static void
on_frame_clock_after_paint (GdkFrameClock *clock,
                            GdkWindow     *window)
{
  gdk_riscos_window_update (window);
}

static void
connect_frame_clock (GdkWindow *window)
{
  if (WINDOW_IS_TOPLEVEL (window))
    {
      GdkFrameClock *frame_clock = gdk_window_get_frame_clock (window);

      g_signal_connect (frame_clock, "after-paint",
                        G_CALLBACK (on_frame_clock_after_paint), window);
    }
}

static void
clear_toplevel_order (void)
{
  GdkWindowImplRiscos *root_impl;

  root_impl = GDK_WINDOW_IMPL_RISCOS (_gdk_root->impl);

  g_list_free (root_impl->sorted_children);
  root_impl->sorted_children = NULL;
}

void
_gdk_riscos_display_create_window_impl (GdkDisplay    *display,
					GdkWindow     *window,
					GdkWindow     *real_parent,
					GdkScreen     *screen,
					GdkEventMask   event_mask,
					GdkWindowAttr *attributes,
					gint           attributes_mask)
{
  GdkWindowImplRiscos *impl;
  GdkWindowImplRiscos *parent_impl;
  GdkRiscosDisplay *riscos_display;

  riscos_display = GDK_RISCOS_DISPLAY (display);

  impl = g_object_new (GDK_TYPE_WINDOW_IMPL_RISCOS, NULL);
  window->impl = (GdkWindowImpl *)impl;
  impl->wrapper = window;
  impl->screen = screen;
  impl->event_mask = event_mask;
  impl->dirty = FALSE;

  gdk_riscos_create_window (window, attributes, attributes_mask);
  g_hash_table_insert (riscos_display->id_ht, GINT_TO_POINTER(impl->id), window);

#if 1
  if (window->window_type != GDK_WINDOW_TOPLEVEL &&
      window->window_type != GDK_WINDOW_TEMP)
    printf("Creating window type %d\n",window->window_type);
  if (GDK_WINDOW_TYPE (window->parent) != GDK_WINDOW_ROOT)
    printf("Attempt to create nested window\n");
#endif

  g_assert (window->window_type == GDK_WINDOW_TOPLEVEL ||
	    window->window_type == GDK_WINDOW_TEMP);
  g_assert (GDK_WINDOW_TYPE (window->parent) == GDK_WINDOW_ROOT);

  riscos_display->toplevels = g_list_prepend (riscos_display->toplevels, impl);

  if (window->window_type == GDK_WINDOW_TOPLEVEL && impl->title)
    {
      const gchar *title;

      title = (attributes_mask & GDK_WA_TITLE) ?
		attributes->title :
		get_default_title ();

      strncpy (impl->title, title, impl->title_max_size);
      impl->title[impl->title_max_size - 1] = '\0';
    }

  impl->min_os_size.x = 0;
  impl->min_os_size.y = 0;

  parent_impl = GDK_WINDOW_IMPL_RISCOS (window->parent->impl);
  
  /* Maintain the z-ordered list of children.
   * The top most children are at the beginning of the list.  */
//  if (window->parent != _gdk_root)
    parent_impl->sorted_children = g_list_prepend (parent_impl->sorted_children, window);
//  else
//    clear_toplevel_order ();

  connect_frame_clock (window);
}

GdkWindow *
gdk_riscos_window_foreign_new_for_display (GdkDisplay      *display,
					   wimp_w          wimp_handle)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);
  GdkWindow *window;
  GdkWindowImplRiscos *impl;

  g_return_val_if_fail (display == _gdk_display, NULL);

  if ((window = gdk_riscos_window_lookup_for_display (display, wimp_handle)) != NULL)
    return g_object_ref (window);

  window = _gdk_display_create_window (display);
  window->visual = gdk_screen_get_system_visual (_gdk_screen);
  window->impl = g_object_new (GDK_TYPE_WINDOW_IMPL_RISCOS, NULL);
  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
  impl->wrapper = window;
  impl->screen = _gdk_screen;
  impl->dirty = FALSE;
  impl->id = wimp_handle;
  g_hash_table_insert (riscos_display->id_ht, GINT_TO_POINTER(impl->id), window);

  // TODO: Get window state and fill in coords.
  window->parent = NULL;
  window->window_type = GDK_WINDOW_FOREIGN;
  window->destroyed = FALSE;
  window->event_mask = GDK_ALL_EVENTS_MASK;
  window->viewable = TRUE;
  window->depth = 24;
  g_object_ref (window);
  
printf("[%s:%s:%d] - wimp_handle (%p)\n",
       __FILE__,__func__,__LINE__,wimp_handle);

  return window;
}

/*static */void
gdk_riscos_window_update (GdkWindow *gdk_window)
{
#ifdef DEBUG_WINDOW_UPDATE
  static unsigned int palette[] = { 0x0000ff00, 0x00ff0000, 0xff000000 };
  static int pal_index = 0;
#endif
  GdkWindowImplRiscos *impl;
  wimp_draw update;
  os_coord origin;
  os_error *err = NULL;
  osbool more;
  int os_height = gdk_riscos_window_os_height(gdk_window);

  impl = GDK_WINDOW_IMPL_RISCOS (gdk_window->impl);
#ifdef DEBUG_WINDOW_UPDATE
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (impl->screen);
#endif

  update.w = impl->id;
  update.box.x0 = 0;
  update.box.y0 = -os_height;
  update.box.x1 = gdk_riscos_window_os_width(gdk_window);
  update.box.y1 = 0;
  xwimp_update_window (&update, &more);

  origin.x = update.box.x0 - update.xscroll;
  origin.y = update.box.y1 - update.yscroll - os_height;

  while (more)
    {
      err = xosspriteop_put_sprite_user_coords (osspriteop_PTR,
						impl->surface_sprite_area,
						(osspriteop_id)impl->surface_sprite_pointer,
						origin.x,
						origin.y,
						os_ACTION_OVERWRITE);
#ifdef DEBUG_WINDOW_UPDATE
      colourtrans_set_gcol (palette[pal_index], 0, 0, NULL);

      os_plot (os_MOVE_TO, origin.x, origin.y);
      os_plot (os_PLOT_BY,
	       gdk_window->width << riscos_screen->x_eigen_factor,
	       gdk_window->height << riscos_screen->y_eigen_factor);
      os_plot (os_MOVE_TO, origin.x, origin.y + (gdk_window->height << riscos_screen->y_eigen_factor));
      os_plot (os_PLOT_BY,
	       gdk_window->width << riscos_screen->x_eigen_factor,
	       -(gdk_window->height << riscos_screen->y_eigen_factor));
#endif

      xwimp_get_rectangle (&update, &more);
    }

#ifdef DEBUG_WINDOW_UPDATE
  if (pal_index >= 2)
    pal_index = 0;
  else
    pal_index++;
#endif

  if (err)
    g_warning ("RISC OS: Failed to render Cairo surface; %s", err->errmess);
}

void
gdk_riscos_window_set_caret (GdkWindow *gdk_window)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (gdk_window->impl);
  os_error *err;

  err = xwimp_set_caret_position (impl->id,
				  wimp_ICON_WINDOW,
				  0, 0,
				  1 << 25, // Flags: Invisible caret.
				  0);
  if (err)
    g_warning("RISC OS: Failed to set caret; %s", err->errmess);
}

void
_gdk_riscos_window_resize_surface (GdkWindow *window)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  if (impl->surface)
    {
#if 0
      cairo_surface_finish (impl->surface);
      cairo_surface_destroy (impl->surface);
      impl->surface = cairo_riscos_sprite_surface_create (CAIRO_FORMAT_RGB24,
						  gdk_window_get_width (impl->wrapper),
						  gdk_window_get_height (impl->wrapper));
#endif

/*      cairo_surface_destroy (impl->surface);

      impl->surface = cairo_riscos_sprite_surface_create (CAIRO_FORMAT_RGB24,
						  gdk_window_get_width (impl->wrapper),
						  gdk_window_get_height (impl->wrapper));*/
#if 1
      cairo_riscos_sprite_surface_set_size (impl->surface,
					    gdk_window_get_width (impl->wrapper),//window->width,
					    gdk_window_get_height (impl->wrapper));//window->height);
#endif
      cairo_riscos_sprite_surface_get_info (impl->surface,
					    (void **)&impl->surface_sprite_area,
					    (void **)&impl->surface_sprite_pointer);
    }
#if 0
  if (impl->ref_surface)
    {
      cairo_surface_set_user_data (impl->ref_surface, &gdk_riscos_cairo_key,
				   NULL, NULL);
      impl->ref_surface = NULL;
    }
#endif
#ifdef DEBUG_WINDOW_SIZE_CHANGED
    {
      char buffer[100];

      sprintf (buffer,"%s: size changed to (%d,%d) - new surface (%p)",
	       __func__,
	       gdk_window_get_width (impl->wrapper),
	       gdk_window_get_height (impl->wrapper),
	       impl->surface);
      report_text0 (buffer);
    }
#endif

  gdk_window_invalidate_rect (window, NULL, TRUE);
//  gdk_window_process_updates (window, TRUE);
}

static void
surface_destroyed (void *data)
{
  GdkWindowImplRiscos *impl = data;

  impl->surface = NULL;
}

static cairo_surface_t *
gdk_window_riscos_ref_cairo_surface (GdkWindow *window)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
  int w, h;

  if (GDK_IS_WINDOW_IMPL_RISCOS (window) &&
      GDK_WINDOW_DESTROYED (impl->wrapper))
    return NULL;

  w = gdk_window_get_width (impl->wrapper);
  h = gdk_window_get_height (impl->wrapper);

  /* Create actual backing store if missing */
  if (!impl->surface)
    {
      impl->surface = cairo_riscos_sprite_surface_create (CAIRO_FORMAT_RGB24, w, h);
      
      cairo_riscos_sprite_surface_get_info (impl->surface,
					    (void **)&impl->surface_sprite_area,
					    (void **)&impl->surface_sprite_pointer);
      cairo_surface_set_user_data (impl->surface, &gdk_riscos_cairo_key,
				   impl, surface_destroyed);
    }

    cairo_surface_reference (impl->surface);
#if 0
  /* Create a destroyable surface referencing the real one */
  if (!impl->ref_surface)
    {
      impl->ref_surface =
	cairo_surface_create_for_rectangle (impl->surface,
					    0, 0,
					    w, h);
      if (impl->ref_surface)
	cairo_surface_set_user_data (impl->ref_surface, &gdk_riscos_cairo_key,
				     impl, ref_surface_destroyed);
    }
//  else
    cairo_surface_reference (impl->surface);
#endif

  return impl->surface;

}

static void
_gdk_riscos_window_destroy (GdkWindow *window,
			    gboolean   recursing,
			    gboolean   foreign_destroy)
{
  GdkWindowImplRiscos *impl;
  GdkRiscosDisplay *riscos_display;
  GdkWindow *parent;

  g_return_if_fail (GDK_IS_WINDOW (window));

  riscos_display = GDK_RISCOS_DISPLAY (gdk_window_get_display (window));
  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

#if 0
  _gdk_riscos_selection_window_destroyed (window);
#endif

  if (riscos_display->pointer_grab.window == window)
    riscos_display->pointer_grab.window = NULL;

  g_list_free (impl->sorted_children);
  impl->sorted_children = NULL;

  parent = window->parent;
  if (parent)
    {
      GdkWindowImplRiscos *parent_impl = GDK_WINDOW_IMPL_RISCOS (parent->impl);

      parent_impl->sorted_children = g_list_remove (parent_impl->sorted_children,
						     window);
    }
#if 0
  if (impl->ref_surface)
    {
      cairo_surface_finish (impl->ref_surface);
      cairo_surface_set_user_data (impl->ref_surface, &gdk_riscos_cairo_key,
				   NULL, NULL);
    }
#endif
  if (impl->surface)
    {
      cairo_surface_set_user_data (impl->surface, &gdk_riscos_cairo_key,
				   NULL, NULL);
      cairo_surface_destroy (impl->surface);
      impl->surface = NULL;
    }

  if (impl->title)
    {
      g_free (impl->title);
      impl->title = NULL;
    }

  g_hash_table_remove (riscos_display->id_ht, GINT_TO_POINTER(impl->id));

  if (impl->id > 0)
    {
      xwimp_delete_window (impl->id);
      impl->id = 0;
    }
}

static cairo_surface_t *
gdk_window_riscos_resize_cairo_surface (GdkWindow       *window,
					cairo_surface_t *surface,
					gint             width,
					gint             height)
{
/*printf ("[%s:%d:%s] - window (%p), resize to (%d, %d)\n",
	__func__, __LINE__, __FILE__,window,width,height);*/

  /* Image surfaces cannot be resized */
//  cairo_surface_destroy (surface);

  return NULL;
}

static void
gdk_riscos_window_destroy_foreign (GdkWindow *window)
{
}

/* This function is called when the XWindow is really gone.
 */
static void
gdk_riscos_window_destroy_notify (GdkWindow *window)
{
  if (!GDK_WINDOW_DESTROYED (window))
    {
      if (GDK_WINDOW_TYPE(window) != GDK_WINDOW_FOREIGN)
	g_warning ("GdkWindow %p unexpectedly destroyed", window);

      _gdk_window_destroy (window, TRUE);
    }

  g_object_unref (window);
}

/* Return TRUE if the window was opened successfully.  */
static gboolean
gdk_riscos_open_window (GdkWindow *gdk_window, GdkWindowImplRiscos *impl)
{
  wimp_open open;
  os_error *err;

  GdkPoint pos;
  pos.x = gdk_window->x;
  pos.y = gdk_window->y;
  gdk_riscos_screen_point_from_pixel (impl->screen, &pos, &pos);  
  
  /* Place the lop left corner of the WIMP window at the position
     stored in the gdk_window.  */
  open.w = impl->id;
  open.visible.x0 = pos.x;
  open.visible.x1 = open.visible.x0 + gdk_riscos_window_os_width(gdk_window);
  open.visible.y1 = gdk_riscos_screen_os_height (impl->screen) - pos.y;
  open.visible.y0 = open.visible.y1 - gdk_riscos_window_os_height(gdk_window);
  open.xscroll = 0;
  open.yscroll = 0;
  open.next = wimp_BACKGROUND;
  err = xwimp_open_window (&open);

  if (err != NULL)
    {
      g_warning ("RISC OS: Failed to open window; %s\n", err->errmess);
      return FALSE;
    }

  /* Work out where the window was actually placed (if, eg, it was forced on screen).  */
  pos.x = open.visible.x0;
  pos.y = gdk_riscos_screen_os_height (impl->screen) - open.visible.y1;
  gdk_riscos_screen_point_to_pixel (impl->screen, &pos, &pos);

  gdk_window->x = pos.x;
  gdk_window->y = pos.y;

  return TRUE;
}

static void
gdk_window_riscos_show (GdkWindow *window, gboolean already_mapped)
{
  GdkWindowImplRiscos *impl;

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  if (gdk_riscos_open_window (window, impl))
    {
      impl->visible = TRUE;

      if (window->event_mask & GDK_STRUCTURE_MASK)
	_gdk_make_event (GDK_WINDOW (window), GDK_MAP, NULL, FALSE);

      if (window->parent && window->parent->event_mask & GDK_SUBSTRUCTURE_MASK)
	_gdk_make_event (GDK_WINDOW (window), GDK_MAP, NULL, FALSE);
    }
#if 0
  riscos_display = GDK_RISCOS_DISPLAY (gdk_window_get_display (window));
  if (riscos_display->output)
    {
      riscos_output_show_surface (riscos_display->output, impl->id);
      queue_dirty_flush (riscos_display);
    }
#endif
}

static void
gdk_window_riscos_hide (GdkWindow *window)
{
  GdkWindowImplRiscos *impl;

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
  impl->visible = FALSE;

  if (window->event_mask & GDK_STRUCTURE_MASK)
    _gdk_make_event (GDK_WINDOW (window), GDK_UNMAP, NULL, FALSE);

  if (window->parent && window->parent->event_mask & GDK_SUBSTRUCTURE_MASK)
    _gdk_make_event (GDK_WINDOW (window), GDK_UNMAP, NULL, FALSE);

  xwimp_close_window (impl->id);
#if 0
  riscos_display = GDK_RISCOS_DISPLAY (gdk_window_get_display (window));
  if (riscos_display->output)
    {
      riscos_output_hide_surface (riscos_display->output, impl->id);
      queue_dirty_flush (riscos_display);
    }

  if (riscos_display->mouse_in_toplevel == window)
    {
      /* TODO: Send leave + enter event, update cursors, etc */
      riscos_display->mouse_in_toplevel = NULL;
    }
#endif
  _gdk_window_clear_update_area (window);
}

static void
gdk_window_riscos_withdraw (GdkWindow *window)
{
  gdk_window_riscos_hide (window);
}

static void
gdk_window_riscos_move_resize (GdkWindow *window,
			       gboolean   with_move,
			       gint       x,
			       gint       y,
			       gint       width,
			       gint       height)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
//  GdkRiscosScreen * = GDK_RISCOS_SCREEN (impl->screen);
  GdkRiscosDisplay *riscos_display;
  gboolean changed, size_changed;
//  gboolean with_resize;

  size_changed = changed = FALSE;

  riscos_display = GDK_RISCOS_DISPLAY (gdk_window_get_display (window));
  if (with_move)
    {
      changed = TRUE;
      window->x = x;
      window->y = y;
    }

//  with_resize = FALSE;
  if (width > 0 || height > 0)
    {
//      with_resize = TRUE;
      if (width < 1)
	width = 1;

      if (height < 1)
	height = 1;

      if (width != window->width ||
	  height != window->height)
	{
	  changed = TRUE;
	  size_changed = TRUE;
#if 0
	  /* Resize clears the content */
	  impl->dirty = TRUE;
	  impl->last_synced = FALSE;
#endif
	  window->width = width;
	  window->height = height;
	  _gdk_riscos_window_resize_surface (window);
	}
    }

  if (changed)
    {
      GdkEvent *event;
      GList *node;

      if (size_changed)
	{
	  /* We don't change the window extent as it's fixed at the screen size.  */
#ifdef DEBUG_WINDOW_SIZE_CHANGED
	  {
	    char buffer[100];

	    sprintf (buffer,"%s: size changed to (%d,%d), window type (%d)",__func__,width,height,window->window_type);
	    report_text0 (buffer);
	  }
#endif
	  window->resize_count++;
	}

      gdk_riscos_open_window (window, impl);

      event = gdk_event_new (GDK_CONFIGURE);
      event->configure.window = g_object_ref (window);
      event->configure.x = window->x;
      event->configure.y = window->y;
      event->configure.width = window->width;
      event->configure.height = window->height;

      gdk_event_set_device (event, GDK_DISPLAY_OBJECT (riscos_display)->core_pointer);

      node = _gdk_event_queue_append (GDK_DISPLAY_OBJECT (riscos_display), event);
      _gdk_windowing_got_event (GDK_DISPLAY_OBJECT (riscos_display), node, event,
				_gdk_display_get_next_serial (GDK_DISPLAY (riscos_display)) - 1);
/*    {
      char buffer[150];
      
      sprintf (buffer, "configure: window (%p), old_pos [%d, %d] old_size [%d, %d]",
	    gwindow,old_pos.x,old_pos.y,old_size.x,old_size.y);
      report_text0 (buffer);
      sprintf (buffer, "configure: window (%p), new_pos [%d, %d] new_size [%d, %d]",
	    gwindow,new_pos.x,new_pos.y,new_size.x,new_size.y);
      report_text0 (buffer);
    }*/
    }
}

static gboolean
gdk_window_riscos_reparent (GdkWindow *window,
			      GdkWindow *new_parent,
			      gint       x,
			      gint       y)
{
  return FALSE;
}

static void
gdk_window_riscos_raise (GdkWindow *window)
{
}

static void
gdk_window_riscos_restack_under (GdkWindow *window,
				   GList *native_siblings /* in requested order, first is bottom-most */)
{
}

static void
gdk_window_riscos_restack_toplevel (GdkWindow *window,
				      GdkWindow *sibling,
				      gboolean   above)
{
}

static void
gdk_window_riscos_lower (GdkWindow *window)
{
}


static void
gdk_riscos_window_focus (GdkWindow *window,
			   guint32    timestamp)
{
}

static void
gdk_riscos_window_set_type_hint (GdkWindow        *window,
				   GdkWindowTypeHint hint)
{
}

static GdkWindowTypeHint
gdk_riscos_window_get_type_hint (GdkWindow *window)
{
  return GDK_WINDOW_TYPE_HINT_NORMAL;
}

static void
gdk_riscos_window_set_modal_hint (GdkWindow *window,
				    gboolean   modal)
{
}

static void
gdk_riscos_window_set_skip_taskbar_hint (GdkWindow *window,
					   gboolean   skips_taskbar)
{
}

static void
gdk_riscos_window_set_skip_pager_hint (GdkWindow *window,
					 gboolean   skips_pager)
{
}

static void
gdk_riscos_window_set_urgency_hint (GdkWindow *window,
				      gboolean   urgent)
{
}

static void
gdk_riscos_window_set_geometry_hints (GdkWindow         *window,
					const GdkGeometry *geometry,
					GdkWindowHints     geom_mask)
{
  GdkWindowImplRiscos *impl;

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  if (geom_mask & GDK_HINT_MIN_SIZE)
    {
      GdkPoint min_size;
      min_size.x = geometry->min_width;
      min_size.y = geometry->min_height;
      gdk_riscos_screen_point_from_pixel (impl->screen,
					  &min_size,
					  &impl->min_os_size);
    }
}

static void
gdk_riscos_window_set_title (GdkWindow   *window,
			     const gchar *title)
{
  GdkWindowImplRiscos *impl;

  g_return_if_fail (title != NULL);

  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL (window))
    return;

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  if (impl->title)
    {
      strncpy (impl->title, title, impl->title_max_size);
      *(impl->title + impl->title_max_size - 1) = '\0';

      if (gdk_window_is_visible (window) && impl->id > 0)
	{
	  /* Need at least RISC OS 3.8 for this.  */
	  xwimp_force_redraw_title (impl->id);
	}
    }
}

static void
gdk_riscos_window_set_role (GdkWindow   *window,
			      const gchar *role)
{
}

static void
gdk_riscos_window_set_startup_id (GdkWindow   *window,
				    const gchar *startup_id)
{
}

static void
gdk_riscos_window_set_transient_for (GdkWindow *window,
				       GdkWindow *parent)
{
//  GdkRiscosDisplay *display;
  GdkWindowImplRiscos *impl;
  wimp_w parent_id;

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  parent_id = 0;
  if (parent)
    parent_id = GDK_WINDOW_IMPL_RISCOS (parent->impl)->id;

  impl->transient_for = parent_id;
#if 0
  display = GDK_RISCOS_DISPLAY (gdk_window_get_display (impl->wrapper));
  if (display->output)
    {
      riscos_output_set_transient_for (display->output, impl->id, impl->transient_for);
      gdk_display_flush (GDK_DISPLAY (display));
    }
#endif
}

static void
gdk_window_riscos_set_background (GdkWindow      *window,
				    cairo_pattern_t *pattern)
{
  return;
}

static void
gdk_window_riscos_set_device_cursor (GdkWindow *window,
				       GdkDevice *device,
				       GdkCursor *cursor)
{
#if 0
  GdkWindowImplRiscos *impl;

  g_return_if_fail (GDK_IS_WINDOW (window));
  g_return_if_fail (GDK_IS_DEVICE (device));

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  if (!cursor)
    g_hash_table_remove (impl->device_cursor, device);
  else
    {
      _gdk_riscos_cursor_update_theme (cursor);
      g_hash_table_replace (impl->device_cursor,
                            device, gdk_cursor_ref (cursor));
    }

  if (!GDK_WINDOW_DESTROYED (window))
    GDK_DEVICE_GET_CLASS (device)->set_window_cursor (device, window, cursor);
#endif
}

GdkCursor *
_gdk_riscos_window_get_cursor (GdkWindow *window)
{
  return NULL;
#if 0
  GdkWindowImplRiscos *impl;

  g_return_val_if_fail (GDK_IS_WINDOW (window), NULL);

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  return impl->cursor;
#endif
}

static void
gdk_window_riscos_get_geometry (GdkWindow *window,
				  gint      *x,
				  gint      *y,
				  gint      *width,
				  gint      *height)
{
  GdkWindowImplRiscos *impl;

  g_return_if_fail (GDK_IS_WINDOW (window));

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  if (x)
    *x = impl->wrapper->x;
  if (y)
    *y = impl->wrapper->y;
  if (width)
    *width = impl->wrapper->width;
  if (height)
    *height = impl->wrapper->height;

}

static gint
gdk_window_riscos_get_root_coords (GdkWindow *window,
				     gint       x,
				     gint       y,
				     gint      *root_x,
				     gint      *root_y)
{
  GdkPoint pos;
  pos.x = x;
  pos.y = y;

  GdkPoint root;
  gdk_riscos_window_map_to_global (window, &pos, &root);

  if (root_x)
    *root_x = root.x;
  if (root_y)
    *root_y = gdk_screen_get_height (gdk_window_get_screen (window)) - root.y;

  return 1;
}

static void
gdk_riscos_window_get_root_origin (GdkWindow *window,
				     gint      *x,
				     gint      *y)
{
  gdk_window_riscos_get_root_coords (window, 0, 0, x, y);
}

static void
gdk_riscos_window_get_frame_extents (GdkWindow    *window,
				       GdkRectangle *rect)
{
  g_return_if_fail (rect != NULL);

  /* TODO: This should take wm frame into account */

  rect->x = window->x;
  rect->y = window->y;
  rect->width = window->width;
  rect->height = window->height;
}

void
gdk_riscos_window_map_from_global (GdkWindow *gwindow,
				   GdkPoint *scr_point,
				   GdkPoint *result)
{
  GdkWindowImplRiscos *impl;
  wimp_window_state state;

  impl = GDK_WINDOW_IMPL_RISCOS (gwindow->impl);

  GdkPoint os_point;
  gdk_riscos_screen_point_from_pixel (impl->screen, scr_point, &os_point);

  state.w = impl->id;
  xwimp_get_window_state (&state);

  os_point.x = os_point.x - (state.visible.x0 - state.xscroll);
  os_point.y = -(os_point.y - (state.visible.y1 - state.yscroll));

  gdk_riscos_screen_point_to_pixel (impl->screen, &os_point, result);
}

void
gdk_riscos_window_map_to_global (GdkWindow *gwindow,
				 GdkPoint *point,
				 GdkPoint *result)
{
  GdkWindowImplRiscos *impl;
  wimp_window_state state;

  impl = GDK_WINDOW_IMPL_RISCOS (gwindow->impl);

  GdkPoint os_point;
  gdk_riscos_screen_point_from_pixel (impl->screen, point, &os_point);

  state.w = impl->id;
  xwimp_get_window_state (&state);

  os_point.x = os_point.x + (state.visible.x0 - state.xscroll);
  os_point.y = -os_point.y + (state.visible.y1 - state.yscroll);

  gdk_riscos_screen_point_to_pixel (impl->screen, &os_point, result);
}

static gboolean
gdk_window_riscos_get_device_state (GdkWindow       *window,
				    GdkDevice       *device,
				    gint            *x,
				    gint            *y,
				    GdkModifierType *mask)
{
  GdkWindow *child;

  g_return_val_if_fail (window == NULL || GDK_IS_WINDOW (window), FALSE);

  if (GDK_WINDOW_DESTROYED (window))
    return FALSE;

  GDK_DEVICE_GET_CLASS (device)->query_state (device, window,
                                              NULL, &child,
                                              NULL, NULL,
                                              x, y, mask);
  return child != NULL;
}

/* NOTE: This function does not appear to be called anywhere.  */
static GdkEventMask
gdk_window_riscos_get_events (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window))
    return 0;
  else
    {
      GdkWindowImplRiscos *impl;

      impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

      return window->event_mask | impl->event_mask;
    }
}

static void
gdk_window_riscos_set_events (GdkWindow    *window,
				GdkEventMask  event_mask)
{
  GdkWindowImplRiscos *impl;

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  impl->event_mask = event_mask;
}

static GdkWindow *
find_child_window_helper (GdkWindow *window,
			  gint       x,
			  gint       y,
			  gint       x_offset,
			  gint       y_offset,
			  gboolean   get_toplevel)
{
  GList *l;

  /* We can't use our own child list as not all child windows will be natively
     nested (currently there are none).  */
  for (l = window->children; l; l = l->next)
    {
      GdkWindow *child = l->data;
      int temp_x, temp_y;

      if (!GDK_WINDOW_IS_MAPPED (child))
	continue;

      temp_x = x_offset + child->x;
      temp_y = y_offset + child->y;

      if ((!get_toplevel || (get_toplevel && window == _gdk_root)) &&
          x >= temp_x && y >= temp_y &&
	  x < temp_x + child->width && y < temp_y + child->height)
	{
	  /* Look for child windows. */
	  return find_child_window_helper (l->data,
					   x, y,
					   temp_x, temp_y,
                                           get_toplevel);
	}
    }
  
  return window;
}

/* Given a GdkWindow and coordinates relative to it, returns the
 * innermost subwindow that contains the point. If the coordinates are
 * outside the passed in window, NULL is returned.
 */
GdkWindow *
_gdk_riscos_window_find_child (GdkWindow *window,
			       gint       x,
			       gint       y,
                               gboolean   get_toplevel)
{
  if (x >= 0 && y >= 0 && x < window->width && y < window->height)
  {
    GdkWindow *temp;
    temp = find_child_window_helper (window, x, y, 0, 0, get_toplevel);
    
    return temp;
  }

  return NULL;
}

static void
gdk_window_riscos_shape_combine_region (GdkWindow       *window,
					  const cairo_region_t *shape_region,
					  gint             offset_x,
					  gint             offset_y)
{
}

static void
gdk_window_riscos_input_shape_combine_region (GdkWindow       *window,
						const cairo_region_t *shape_region,
						gint             offset_x,
						gint             offset_y)
{
}


static void
gdk_riscos_window_set_override_redirect (GdkWindow *window,
					   gboolean override_redirect)
{
}

static void
gdk_riscos_window_set_accept_focus (GdkWindow *window,
				      gboolean accept_focus)
{
  accept_focus = accept_focus != FALSE;

  if (window->accept_focus != accept_focus)
    {
      window->accept_focus = accept_focus;
    }
}

static void
gdk_riscos_window_set_focus_on_map (GdkWindow *window,
				      gboolean focus_on_map)
{
  focus_on_map = focus_on_map != FALSE;

  if (window->focus_on_map != focus_on_map)
    {
      window->focus_on_map = focus_on_map;
    }
}


static void
gdk_riscos_window_set_icon_list (GdkWindow *window,
				   GList     *pixbufs)
{
}

static void
gdk_riscos_window_set_icon_name (GdkWindow   *window,
				   const gchar *name)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

  g_object_set_qdata (G_OBJECT (window), g_quark_from_static_string ("gdk-icon-name-set"),
		      GUINT_TO_POINTER (name != NULL));
}

static void
gdk_riscos_window_iconify (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;
}

static void
gdk_riscos_window_deiconify (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;
}

static void
gdk_riscos_window_stick (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static void
gdk_riscos_window_unstick (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static void
gdk_riscos_window_maximize (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static void
gdk_riscos_window_unmaximize (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static void
gdk_riscos_window_fullscreen (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static void
gdk_riscos_window_unfullscreen (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static void
gdk_riscos_window_set_keep_above (GdkWindow *window,
				    gboolean   setting)
{
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static void
gdk_riscos_window_set_keep_below (GdkWindow *window, gboolean setting)
{
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static GdkWindow *
gdk_riscos_window_get_group (GdkWindow *window)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL (window))
    return NULL;

  return window;
}

static void
gdk_riscos_window_set_group (GdkWindow *window,
			       GdkWindow *leader)
{
}

static void
gdk_riscos_window_set_decorations (GdkWindow      *window,
				     GdkWMDecoration decorations)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

}

static gboolean
gdk_riscos_window_get_decorations (GdkWindow       *window,
				     GdkWMDecoration *decorations)
{
  gboolean result = FALSE;

  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return FALSE;

  return result;
}

static void
gdk_riscos_window_set_functions (GdkWindow    *window,
				   GdkWMFunction functions)
{
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;
}

static cairo_region_t *
gdk_riscos_window_get_shape (GdkWindow *window)
{
  return NULL;
}

static cairo_region_t *
gdk_riscos_window_get_input_shape (GdkWindow *window)
{
  return NULL;
}


static gboolean
gdk_window_riscos_set_static_gravities (GdkWindow *window,
					  gboolean   use_static)
{
  return TRUE;
}

#if 0

typedef struct _MoveResizeData MoveResizeData;

struct _MoveResizeData
{
  GdkDisplay *display;

  GdkWindow *moveresize_window;
  GdkWindow *moveresize_emulation_window;
  gboolean is_resize;
  GdkWindowEdge resize_edge;
  gint moveresize_button;
  gint moveresize_x;
  gint moveresize_y;
  gint moveresize_orig_x;
  gint moveresize_orig_y;
  gint moveresize_orig_width;
  gint moveresize_orig_height;
  long moveresize_process_time;
  RiscosInputMsg *moveresize_pending_event;
};

static MoveResizeData *
get_move_resize_data (GdkDisplay *display,
		      gboolean    create)
{
  MoveResizeData *mv_resize;
  static GQuark move_resize_quark = 0;

  if (!move_resize_quark)
    move_resize_quark = g_quark_from_static_string ("gdk-window-moveresize");

  mv_resize = g_object_get_qdata (G_OBJECT (display), move_resize_quark);

  if (!mv_resize && create)
    {
      mv_resize = g_new0 (MoveResizeData, 1);
      mv_resize->display = display;

      g_object_set_qdata (G_OBJECT (display), move_resize_quark, mv_resize);
    }

  return mv_resize;
}

static void
update_pos (MoveResizeData *mv_resize,
	    gint            new_root_x,
	    gint            new_root_y)
{
  gint dx, dy;

  dx = new_root_x - mv_resize->moveresize_x;
  dy = new_root_y - mv_resize->moveresize_y;

  if (mv_resize->is_resize)
    {
      gint x, y, w, h;

      x = mv_resize->moveresize_orig_x;
      y = mv_resize->moveresize_orig_y;

      w = mv_resize->moveresize_orig_width;
      h = mv_resize->moveresize_orig_height;

      switch (mv_resize->resize_edge)
	{
	case GDK_WINDOW_EDGE_NORTH_WEST:
	  x += dx;
	  y += dy;
	  w -= dx;
	  h -= dy;
	  break;
	case GDK_WINDOW_EDGE_NORTH:
	  y += dy;
	  h -= dy;
	  break;
	case GDK_WINDOW_EDGE_NORTH_EAST:
	  y += dy;
	  h -= dy;
	  w += dx;
	  break;
	case GDK_WINDOW_EDGE_SOUTH_WEST:
	  h += dy;
	  x += dx;
	  w -= dx;
	  break;
	case GDK_WINDOW_EDGE_SOUTH_EAST:
	  w += dx;
	  h += dy;
	  break;
	case GDK_WINDOW_EDGE_SOUTH:
	  h += dy;
	  break;
	case GDK_WINDOW_EDGE_EAST:
	  w += dx;
	  break;
	case GDK_WINDOW_EDGE_WEST:
	  x += dx;
	  w -= dx;
	  break;
	}

      x = MAX (x, 0);
      y = MAX (y, 0);
      w = MAX (w, 1);
      h = MAX (h, 1);

      gdk_window_move_resize (mv_resize->moveresize_window, x, y, w, h);
    }
  else
    {
      gint x, y;

      x = mv_resize->moveresize_orig_x + dx;
      y = mv_resize->moveresize_orig_y + dy;

      gdk_window_move (mv_resize->moveresize_window, x, y);
    }
}

static void
finish_drag (MoveResizeData *mv_resize)
{
  gdk_window_destroy (mv_resize->moveresize_emulation_window);
  mv_resize->moveresize_emulation_window = NULL;
  g_object_unref (mv_resize->moveresize_window);
  mv_resize->moveresize_window = NULL;

  if (mv_resize->moveresize_pending_event)
    {
      g_free (mv_resize->moveresize_pending_event);
      mv_resize->moveresize_pending_event = NULL;
    }
}

static gboolean
moveresize_lookahead (GdkDisplay *display,
		      MoveResizeData *mv_resize,
		      RiscosInputMsg *event)
{
  GdkRiscosDisplay *riscos_display;
  RiscosInputMsg *message;
  GList *l;

  riscos_display = GDK_RISCOS_DISPLAY (display);
  for (l = riscos_display->input_messages; l != NULL; l = l->next)
    {
      message = l->data;
      if (message->base.type == 'm')
	return FALSE;
      if (message->base.type == 'b')
	return FALSE;
    }

  return TRUE;
}

gboolean
_gdk_riscos_moveresize_handle_event (GdkDisplay *display,
				       RiscosInputMsg *event)
{
  guint button_mask = 0;
  MoveResizeData *mv_resize = get_move_resize_data (display, FALSE);

  if (!mv_resize || !mv_resize->moveresize_window)
    return FALSE;

  button_mask = GDK_BUTTON1_MASK << (mv_resize->moveresize_button - 1);

  switch (event->base.type)
    {
    case 'm':
      if (mv_resize->moveresize_window->resize_count > 0)
	{
	  if (mv_resize->moveresize_pending_event)
	    *mv_resize->moveresize_pending_event = *event;
	  else
	    mv_resize->moveresize_pending_event =
	      g_memdup (event, sizeof (RiscosInputMsg));

	  break;
	}
      if (!moveresize_lookahead (display, mv_resize, event))
	break;

      update_pos (mv_resize,
		  event->pointer.root_x,
		  event->pointer.root_y);

      /* This should never be triggered in normal cases, but in the
       * case where the drag started without an implicit grab being
       * in effect, we could miss the release if it occurs before
       * we grab the pointer; this ensures that we will never
       * get a permanently stuck grab.
       */
      if ((event->pointer.state & button_mask) == 0)
	finish_drag (mv_resize);
      break;

    case 'B':
      update_pos (mv_resize,
		  event->pointer.root_x,
		  event->pointer.root_y);

      if (event->button.button == mv_resize->moveresize_button)
	finish_drag (mv_resize);
      break;
    }
  return TRUE;
}

gboolean
_gdk_riscos_moveresize_configure_done (GdkDisplay *display,
					 GdkWindow  *window)
{
  RiscosInputMsg *tmp_event;
  MoveResizeData *mv_resize = get_move_resize_data (display, FALSE);

  if (!mv_resize || window != mv_resize->moveresize_window)
    return FALSE;

  if (mv_resize->moveresize_pending_event)
    {
      tmp_event = mv_resize->moveresize_pending_event;
      mv_resize->moveresize_pending_event = NULL;
      _gdk_riscos_moveresize_handle_event (display, tmp_event);
      g_free (tmp_event);
    }

  return TRUE;
}

static void
create_moveresize_window (MoveResizeData *mv_resize,
			  guint32         timestamp)
{
  GdkWindowAttr attributes;
  gint attributes_mask;
  GdkGrabStatus status;

  g_assert (mv_resize->moveresize_emulation_window == NULL);

  attributes.x = -100;
  attributes.y = -100;
  attributes.width = 10;
  attributes.height = 10;
  attributes.window_type = GDK_WINDOW_TEMP;
  attributes.wclass = GDK_INPUT_ONLY;
  attributes.override_redirect = TRUE;
  attributes.event_mask = 0;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_NOREDIR;

  mv_resize->moveresize_emulation_window =
    gdk_window_new (gdk_screen_get_root_window (gdk_display_get_default_screen (mv_resize->display)),
		    &attributes,
		    attributes_mask);

  gdk_window_show (mv_resize->moveresize_emulation_window);

  status = gdk_pointer_grab (mv_resize->moveresize_emulation_window,
			     FALSE,
			     GDK_BUTTON_RELEASE_MASK |
			     GDK_POINTER_MOTION_MASK,
			     NULL,
			     NULL,
			     timestamp);

  if (status != GDK_GRAB_SUCCESS)
    {
      /* If this fails, some other client has grabbed the window
       * already.
       */
      finish_drag (mv_resize);
    }

  mv_resize->moveresize_process_time = 0;
}
#endif
static void
gdk_riscos_window_begin_resize_drag (GdkWindow     *window,
				       GdkWindowEdge  edge,
                                       GdkDevice     *device,
				       gint           button,
				       gint           root_x,
				       gint           root_y,
				       guint32        timestamp)
{
#if 0
  GdkRiscosDisplay *riscos_display;
  MoveResizeData *mv_resize;

  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL_OR_FOREIGN (window))
    return;

  /* We need a connection to be able to get mouse events, if not, punt */
  riscos_display = GDK_RISCOS_DISPLAY (gdk_window_get_display (window));
  if (!riscos_display->output)
    return;

  mv_resize = get_move_resize_data (GDK_WINDOW_DISPLAY (window), TRUE);

  mv_resize->is_resize = TRUE;
  mv_resize->moveresize_button = button;
  mv_resize->resize_edge = edge;
  mv_resize->moveresize_x = root_x;
  mv_resize->moveresize_y = root_y;
  mv_resize->moveresize_window = g_object_ref (window);

  gdk_window_get_origin (mv_resize->moveresize_window,
			 &mv_resize->moveresize_orig_x,
			 &mv_resize->moveresize_orig_y);
  mv_resize->moveresize_orig_width = gdk_window_get_width (window);
  mv_resize->moveresize_orig_height = gdk_window_get_height (window);

  create_moveresize_window (mv_resize, timestamp);
#endif
}

static void
gdk_riscos_window_begin_move_drag (GdkWindow *window,
                                     GdkDevice *device,
				     gint       button,
				     gint       root_x,
				     gint       root_y,
				     guint32    timestamp)
{
  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL (window))
    return;

}

static void
gdk_riscos_window_enable_synchronized_configure (GdkWindow *window)
{
  if (!GDK_IS_WINDOW_IMPL_RISCOS (window->impl))
    return;
}

static void
gdk_riscos_window_configure_finished (GdkWindow *window)
{
  if (!WINDOW_IS_TOPLEVEL (window))
    return;
}

static gboolean
gdk_riscos_window_beep (GdkWindow *window)
{
  return FALSE;
}

static void
gdk_riscos_window_set_opacity (GdkWindow *window,
				 gdouble    opacity)
{
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (GDK_WINDOW_DESTROYED (window) ||
      !WINDOW_IS_TOPLEVEL (window))
    return;

  if (opacity < 0)
    opacity = 0;
  else if (opacity > 1)
    opacity = 1;
}

static void
gdk_riscos_window_set_composited (GdkWindow *window,
				  gboolean   composited)
{
}
/*
static gboolean
update_idle (gpointer data)
{
  GdkWindow *window = data;

  if (!GDK_WINDOW_DESTROYED (window))
  {
    GdkWindowImplRiscos *impl;
    
    impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
    if (impl->dirty)
    {
      gdk_riscos_window_update (window);
      impl->dirty = FALSE;
    }
  }

  return TRUE;
}
*/
static void
gdk_riscos_window_process_updates_recurse (GdkWindow *window,
					   cairo_region_t *region)
{
  GdkWindowImplRiscos *impl;

  _gdk_window_process_updates_recurse (window, region);

  impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
  impl->dirty = TRUE;

//  gdk_threads_add_idle (update_idle, window);
}

gboolean
_gdk_riscos_window_queue_antiexpose (GdkWindow *window,
				       cairo_region_t *area)
{
  return TRUE;
}

/* This code is taken from the offscreen target surface.  */
void
_gdk_riscos_window_translate (GdkWindow      *window,
			      cairo_region_t *area,
			      gint            dx,
			      gint            dy)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  if (impl->surface)
    {
      cairo_t *cr;

      cr = cairo_create (impl->surface);

      area = cairo_region_copy (area);

      gdk_cairo_region (cr, area);
      cairo_clip (cr);

      /* NB: This is a self-copy and Cairo doesn't support that yet.
       * So we do a litle trick.
       */
      cairo_push_group (cr);

      cairo_set_source_surface (cr, impl->surface, dx, dy);
      cairo_paint (cr);

      cairo_pop_group_to_source (cr);
      cairo_paint (cr);

      cairo_destroy (cr);
    }

  _gdk_window_add_damage (window, area);
}
#if 0
guint32
gdk_riscos_get_last_seen_time (GdkWindow  *window)
{
  GdkDisplay *display;

  display = gdk_window_get_display (window);
  _gdk_riscos_display_consume_all_input (display);
  return (guint32) GDK_RISCOS_DISPLAY (display)->last_seen_time;
}
#endif

static gboolean
gdk_riscos_window_simulate_key (GdkWindow      *window,
				gint            x,
				gint            y,
				guint           keyval,
				GdkModifierType modifiers,
				GdkEventType    key_pressrelease)
{
  return FALSE;
}

static gboolean
gdk_riscos_window_simulate_button (GdkWindow      *window,
				   gint            x,
				   gint            y,
				   guint           button, /*1..3*/
				   GdkModifierType modifiers,
				   GdkEventType    button_pressrelease)
{
  return FALSE;
}

static void
gdk_riscos_window_sync_rendering (GdkWindow *window)
{
  char buffer[100];
  sprintf (buffer, "\\R%s",__func__);
  report_text0(buffer);
}

void
gdk_riscos_window_dump_to_file (GdkWindow *window,
				const char *file)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  cairo_riscos_sprite_surface_write_to_file (impl->surface, file);
}

wimp_w
gdk_riscos_window_get_handle (GdkWindow *window)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);

  return impl->id;
}

static void
gdk_window_impl_riscos_class_init (GdkWindowImplRiscosClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GdkWindowImplClass *impl_class = GDK_WINDOW_IMPL_CLASS (klass);

  object_class->finalize = gdk_window_impl_riscos_finalize;

  impl_class->ref_cairo_surface = gdk_window_riscos_ref_cairo_surface;
  impl_class->show = gdk_window_riscos_show;
  impl_class->hide = gdk_window_riscos_hide;
  impl_class->withdraw = gdk_window_riscos_withdraw;
  impl_class->set_events = gdk_window_riscos_set_events;
  impl_class->get_events = gdk_window_riscos_get_events;
  impl_class->raise = gdk_window_riscos_raise;
  impl_class->lower = gdk_window_riscos_lower;
  impl_class->restack_under = gdk_window_riscos_restack_under;
  impl_class->restack_toplevel = gdk_window_riscos_restack_toplevel;
  impl_class->move_resize = gdk_window_riscos_move_resize;
  impl_class->set_background = gdk_window_riscos_set_background;
  impl_class->reparent = gdk_window_riscos_reparent;
  impl_class->set_device_cursor = gdk_window_riscos_set_device_cursor;
  impl_class->get_geometry = gdk_window_riscos_get_geometry;
  impl_class->get_root_coords = gdk_window_riscos_get_root_coords;
  impl_class->get_device_state = gdk_window_riscos_get_device_state;
  impl_class->shape_combine_region = gdk_window_riscos_shape_combine_region;
  impl_class->input_shape_combine_region = gdk_window_riscos_input_shape_combine_region;
  impl_class->set_static_gravities = gdk_window_riscos_set_static_gravities;
  impl_class->queue_antiexpose = _gdk_riscos_window_queue_antiexpose;
//  impl_class->translate = _gdk_riscos_window_translate;
  impl_class->destroy = _gdk_riscos_window_destroy;
  impl_class->destroy_foreign = gdk_riscos_window_destroy_foreign;
  impl_class->resize_cairo_surface = gdk_window_riscos_resize_cairo_surface;
  impl_class->get_shape = gdk_riscos_window_get_shape;
  impl_class->get_input_shape = gdk_riscos_window_get_input_shape;
  impl_class->beep = gdk_riscos_window_beep;

  impl_class->focus = gdk_riscos_window_focus;
  impl_class->set_type_hint = gdk_riscos_window_set_type_hint;
  impl_class->get_type_hint = gdk_riscos_window_get_type_hint;
  impl_class->set_modal_hint = gdk_riscos_window_set_modal_hint;
  impl_class->set_skip_taskbar_hint = gdk_riscos_window_set_skip_taskbar_hint;
  impl_class->set_skip_pager_hint = gdk_riscos_window_set_skip_pager_hint;
  impl_class->set_urgency_hint = gdk_riscos_window_set_urgency_hint;
  impl_class->set_geometry_hints = gdk_riscos_window_set_geometry_hints;
  impl_class->set_title = gdk_riscos_window_set_title;
  impl_class->set_role = gdk_riscos_window_set_role;
  impl_class->set_startup_id = gdk_riscos_window_set_startup_id;
  impl_class->set_transient_for = gdk_riscos_window_set_transient_for;
  impl_class->get_root_origin = gdk_riscos_window_get_root_origin;
  impl_class->get_frame_extents = gdk_riscos_window_get_frame_extents;
  impl_class->set_override_redirect = gdk_riscos_window_set_override_redirect;
  impl_class->set_accept_focus = gdk_riscos_window_set_accept_focus;
  impl_class->set_focus_on_map = gdk_riscos_window_set_focus_on_map;
  impl_class->set_icon_list = gdk_riscos_window_set_icon_list;
  impl_class->set_icon_name = gdk_riscos_window_set_icon_name;
  impl_class->iconify = gdk_riscos_window_iconify;
  impl_class->deiconify = gdk_riscos_window_deiconify;
  impl_class->stick = gdk_riscos_window_stick;
  impl_class->unstick = gdk_riscos_window_unstick;
  impl_class->maximize = gdk_riscos_window_maximize;
  impl_class->unmaximize = gdk_riscos_window_unmaximize;
  impl_class->fullscreen = gdk_riscos_window_fullscreen;
  impl_class->unfullscreen = gdk_riscos_window_unfullscreen;
  impl_class->set_keep_above = gdk_riscos_window_set_keep_above;
  impl_class->set_keep_below = gdk_riscos_window_set_keep_below;
  impl_class->get_group = gdk_riscos_window_get_group;
  impl_class->set_group = gdk_riscos_window_set_group;
  impl_class->set_decorations = gdk_riscos_window_set_decorations;
  impl_class->get_decorations = gdk_riscos_window_get_decorations;
  impl_class->set_functions = gdk_riscos_window_set_functions;
  impl_class->set_functions = gdk_riscos_window_set_functions;
  impl_class->begin_resize_drag = gdk_riscos_window_begin_resize_drag;
  impl_class->begin_move_drag = gdk_riscos_window_begin_move_drag;
  impl_class->enable_synchronized_configure = gdk_riscos_window_enable_synchronized_configure;
  impl_class->configure_finished = gdk_riscos_window_configure_finished;
  impl_class->set_opacity = gdk_riscos_window_set_opacity;
  impl_class->set_composited = gdk_riscos_window_set_composited;
  impl_class->destroy_notify = gdk_riscos_window_destroy_notify;
  impl_class->register_dnd = _gdk_riscos_window_register_dnd;
  impl_class->drag_begin = _gdk_riscos_window_drag_begin;
  impl_class->process_updates_recurse = gdk_riscos_window_process_updates_recurse;
  impl_class->sync_rendering = gdk_riscos_window_sync_rendering;
  impl_class->simulate_key = gdk_riscos_window_simulate_key;
  impl_class->simulate_button = gdk_riscos_window_simulate_button;
  impl_class->get_property = _gdk_riscos_window_get_property;
  impl_class->change_property = _gdk_riscos_window_change_property;
  impl_class->delete_property = _gdk_riscos_window_delete_property;
  impl_class->get_drag_protocol = _gdk_riscos_window_get_drag_protocol;
}
