/*
 * gdkscreen-riscos.h
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

#ifndef __GDK_RISCOS_SCREEN_H__
#define __GDK_RISCOS_SCREEN_H__

#include <gdk/gdkscreenprivate.h>
#include <gdk/gdkvisual.h>
#include <gdk/gdkinternals.h>
#include "oslib/wimp.h"

G_BEGIN_DECLS

typedef struct _GdkRiscosScreen GdkRiscosScreen;
typedef struct _GdkRiscosScreenClass GdkRiscosScreenClass;

#define GDK_TYPE_RISCOS_SCREEN              (gdk_riscos_screen_get_type ())
#define GDK_RISCOS_SCREEN(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_RISCOS_SCREEN, GdkRiscosScreen))
#define GDK_RISCOS_SCREEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_RISCOS_SCREEN, GdkRiscosScreenClass))
#define GDK_IS_RISCOS_SCREEN(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_RISCOS_SCREEN))
#define GDK_IS_RISCOS_SCREEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_RISCOS_SCREEN))
#define GDK_RISCOS_SCREEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_RISCOS_SCREEN, GdkRiscosScreenClass))

typedef struct _GdkRiscosMonitor GdkRiscosMonitor;

struct _GdkRiscosScreen
{
  GdkScreen parent_instance;

  GdkDisplay *display;
  GdkWindow *root_window;

  int width;
  int height;

  int x_eigen_factor;
  int y_eigen_factor;

  /* Visual Part */
  GdkVisual **visuals;
  gint nvisuals;
  GdkVisual *system_visual;
  GdkVisual *rgba_visual;
  gint available_depths[7];
  gint navailable_depths;
  GdkVisualType available_types[6];
  gint navailable_types;
};

struct _GdkRiscosScreenClass
{
  GdkScreenClass parent_class;

  void (* window_manager_changed) (GdkRiscosScreen *screen);
};

GType       gdk_riscos_screen_get_type (void);
GdkScreen * _gdk_riscos_screen_new      (GdkDisplay *display,
					   gint	  screen_number);
void _gdk_riscos_screen_setup           (GdkScreen *screen);

static inline int
gdk_riscos_screen_os_width (GdkScreen *screen)
{
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (screen);

  return (rscreen->width << rscreen->x_eigen_factor);
}

static inline int
gdk_riscos_screen_os_height (GdkScreen *screen)
{
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (screen);

  return (rscreen->height << rscreen->y_eigen_factor);
}

static inline void
gdk_riscos_screen_point_to_pixel (GdkScreen *screen,
				  GdkPoint *os_point,
				  GdkPoint *result)
{
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (screen);

  result->x = os_point->x >> rscreen->x_eigen_factor;
  result->y = os_point->y >> rscreen->y_eigen_factor;
}

static inline void
gdk_riscos_screen_point_from_pixel (GdkScreen *screen,
				    GdkPoint *pixel_point,
				    GdkPoint *result)
{
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (screen);

  result->x = pixel_point->x << rscreen->x_eigen_factor;
  result->y = pixel_point->y << rscreen->y_eigen_factor;
}

static inline void
gdk_riscos_screen_global_point (GdkScreen *screen,
				GdkPoint *point,
				GdkPoint *result)
{
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (screen);

  result->x = point->x;
  result->y = rscreen->height - point->y;
}

static inline int
gdk_riscos_screen_os_x (GdkScreen *screen,
			int pixel_x)
{
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (screen);

  return pixel_x << rscreen->x_eigen_factor;
}

static inline int
gdk_riscos_screen_os_y (GdkScreen *screen,
			int pixel_y)
{
  GdkRiscosScreen *rscreen = GDK_RISCOS_SCREEN (screen);

  return pixel_y << rscreen->y_eigen_factor;
}

G_END_DECLS

#endif /* __GDK_RISCOS_SCREEN_H__ */
