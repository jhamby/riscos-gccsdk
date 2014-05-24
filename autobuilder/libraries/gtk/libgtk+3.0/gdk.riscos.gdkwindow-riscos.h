/* GDK - The GIMP Drawing Kit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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

#ifndef __GDK_WINDOW_RISCOS_H__
#define __GDK_WINDOW_RISCOS_H__

#include <gdk/gdkwindowimpl.h>
#include "gdkscreen-riscos.h"
#include "oslib/wimp.h"
#include "oslib/osspriteop.h"

#ifndef wimp_INVALID_WINDOW
#define wimp_INVALID_WINDOW (wimp_w)0xFFFFFFFF
#endif

G_BEGIN_DECLS

typedef struct _GdkWindowImplRiscos GdkWindowImplRiscos;
typedef struct _GdkWindowImplRiscosClass GdkWindowImplRiscosClass;

/* Window implementation for RISC OS
 */

#define GDK_TYPE_WINDOW_IMPL_RISCOS              (gdk_window_impl_riscos_get_type ())
#define GDK_WINDOW_IMPL_RISCOS(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_WINDOW_IMPL_RISCOS, GdkWindowImplRiscos))
#define GDK_WINDOW_IMPL_RISCOS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_WINDOW_IMPL_RISCOS, GdkWindowImplRiscosClass))
#define GDK_IS_WINDOW_IMPL_RISCOS(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_WINDOW_IMPL_RISCOS))
#define GDK_IS_WINDOW_IMPL_RISCOS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_WINDOW_IMPL_RISCOS))
#define GDK_WINDOW_IMPL_RISCOS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_WINDOW_IMPL_RISCOS, GdkWindowImplRiscosClass))

struct _GdkWindowImplRiscos
{
  GdkWindowImpl parent_instance;

  GdkWindow *wrapper;
  GdkScreen *screen;

  cairo_surface_t *surface;

  osspriteop_area *surface_sprite_area;
  osspriteop_header *surface_sprite_pointer;

  wimp_w id;
  gchar *title;
  int title_max_size;
  gchar *title_validation;

  GdkPoint min_os_size;

  gboolean visible;
  wimp_w transient_for;

  gint8 toplevel_window_type;

  gboolean dirty;

  /* Sorted by z-order */
  GList *sorted_children;

  GdkEventMask event_mask;
#if 0
  GdkCursor *cursor;
  GHashTable *device_cursor;

  gboolean last_synced;
#endif
};

struct _GdkWindowImplRiscosClass
{
  GdkWindowImplClass parent_class;
};

GType
gdk_window_impl_riscos_get_type (void);

void
gdk_riscos_window_update (GdkWindow *);

GdkWindow *
gdk_riscos_window_foreign_new_for_display (GdkDisplay      *display,
					   wimp_w          wimp_handle);

void
gdk_riscos_window_map_from_global (GdkWindow *gwindow,
				   GdkPoint *point,
				   GdkPoint *result);
void
gdk_riscos_window_map_to_global (GdkWindow *gwindow,
				 GdkPoint *point,
				 GdkPoint *result);

void
gdk_riscos_window_set_caret (GdkWindow *gdk_window);

static inline int
gdk_riscos_window_os_width (GdkWindow *gdk_window)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (gdk_window->impl);
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (impl->screen);

  return gdk_window->width << rscreen->x_eigen_factor;
}

static inline int
gdk_riscos_window_os_height (GdkWindow *gdk_window)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (gdk_window->impl);
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (impl->screen);

  return gdk_window->height << rscreen->y_eigen_factor;
}

G_END_DECLS

#endif /* __GDK_WINDOW_RISCOS_H__ */
