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

#include "config.h"

#include "gdkvisualprivate.h"

#include "gdkprivate-riscos.h"
#include "gdkscreen-riscos.h"
#include "gdkinternals.h"

struct _GdkRiscosVisual
{
  GdkVisual visual;
};

struct _GdkRiscosVisualClass
{
  GObjectClass parent_class;
};

static void     gdk_visual_decompose_mask (gulong     mask,
					   gint      *shift,
					   gint      *prec);


G_DEFINE_TYPE (GdkRiscosVisual, gdk_riscos_visual, GDK_TYPE_VISUAL)

static void
gdk_riscos_visual_finalize (GObject *object)
{
  G_OBJECT_CLASS (gdk_riscos_visual_parent_class)->finalize (object);
}

static void
gdk_riscos_visual_class_init (GdkRiscosVisualClass *visual_class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (visual_class);

  object_class->finalize = gdk_riscos_visual_finalize;
}

static void
gdk_riscos_visual_init (GdkRiscosVisual *visual)
{
}

void
_gdk_riscos_screen_init_visuals (GdkScreen *screen)
{
  GdkRiscosScreen *riscos_screen;
  GdkVisual **visuals;
  int nvisuals;

  g_return_if_fail (GDK_IS_SCREEN (screen));
  riscos_screen = GDK_RISCOS_SCREEN (screen);

  nvisuals = 2;
  visuals = g_new (GdkVisual *, nvisuals);

  visuals[0] = g_object_new (GDK_TYPE_RISCOS_VISUAL, NULL);
  visuals[0]->screen = screen;
  visuals[0]->type = GDK_VISUAL_TRUE_COLOR;
  visuals[0]->depth = 32;
  visuals[0]->byte_order = (G_BYTE_ORDER == G_LITTLE_ENDIAN) ? GDK_LSB_FIRST : GDK_MSB_FIRST;
  visuals[0]->red_mask = 0xff0000;
  visuals[0]->green_mask = 0xff00;
  visuals[0]->blue_mask = 0xff;
  visuals[0]->colormap_size = 256;
  visuals[0]->bits_per_rgb = 8;
  gdk_visual_decompose_mask (visuals[0]->red_mask,
			     &visuals[0]->red_shift,
			     &visuals[0]->red_prec);
  gdk_visual_decompose_mask (visuals[0]->green_mask,
			     &visuals[0]->green_shift,
			     &visuals[0]->green_prec);
  gdk_visual_decompose_mask (visuals[0]->blue_mask,
			     &visuals[0]->blue_shift,
			     &visuals[0]->blue_prec);

  visuals[1] = g_object_new (GDK_TYPE_RISCOS_VISUAL, NULL);
  visuals[1]->screen = screen;
  visuals[1]->type = GDK_VISUAL_TRUE_COLOR;
  visuals[1]->depth = 24;
  visuals[1]->byte_order = (G_BYTE_ORDER == G_LITTLE_ENDIAN) ? GDK_LSB_FIRST : GDK_MSB_FIRST;
  visuals[1]->red_mask = 0xff0000;
  visuals[1]->green_mask = 0xff00;
  visuals[1]->blue_mask = 0xff;
  visuals[1]->colormap_size = 256;
  visuals[1]->bits_per_rgb = 8;
  gdk_visual_decompose_mask (visuals[1]->red_mask,
			     &visuals[1]->red_shift,
			     &visuals[1]->red_prec);
  gdk_visual_decompose_mask (visuals[1]->green_mask,
			     &visuals[1]->green_shift,
			     &visuals[1]->green_prec);
  gdk_visual_decompose_mask (visuals[1]->blue_mask,
			     &visuals[1]->blue_shift,
			     &visuals[1]->blue_prec);

  riscos_screen->system_visual = visuals[1];
  riscos_screen->rgba_visual = visuals[0];

  riscos_screen->navailable_depths = 2;
  riscos_screen->available_depths[0] = 32;
  riscos_screen->available_depths[1] = 24;

  riscos_screen->navailable_types = 1;
  riscos_screen->available_types[0] = GDK_VISUAL_TRUE_COLOR;

  riscos_screen->visuals = visuals;
  riscos_screen->nvisuals = nvisuals;
}

gint
_gdk_riscos_screen_visual_get_best_depth (GdkScreen * screen)
{
  return GDK_RISCOS_SCREEN (screen)->available_depths[0];
}

GdkVisualType
_gdk_riscos_screen_visual_get_best_type (GdkScreen * screen)
{
  return GDK_RISCOS_SCREEN (screen)->available_types[0];
}

GdkVisual *
_gdk_riscos_screen_get_system_visual (GdkScreen * screen)
{
  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);

  return ((GdkVisual *) GDK_RISCOS_SCREEN (screen)->system_visual);
}

GdkVisual*
_gdk_riscos_screen_visual_get_best (GdkScreen * screen)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);

  return (GdkVisual *)riscos_screen->visuals[0];
}

GdkVisual*
_gdk_riscos_screen_visual_get_best_with_depth (GdkScreen * screen,
						 gint depth)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);
  GdkVisual *return_val;
  int i;

  return_val = NULL;
  for (i = 0; i < riscos_screen->nvisuals; i++)
    if (depth == riscos_screen->visuals[i]->depth)
      {
	return_val = (GdkVisual *) riscos_screen->visuals[i];
	break;
      }

  return return_val;
}

GdkVisual*
_gdk_riscos_screen_visual_get_best_with_type (GdkScreen * screen,
						GdkVisualType visual_type)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);
  GdkVisual *return_val;
  int i;

  return_val = NULL;
  for (i = 0; i < riscos_screen->nvisuals; i++)
    if (visual_type == riscos_screen->visuals[i]->type)
      {
	return_val = (GdkVisual *) riscos_screen->visuals[i];
	break;
      }

  return return_val;
}

GdkVisual*
_gdk_riscos_screen_visual_get_best_with_both (GdkScreen * screen,
						gint          depth,
						GdkVisualType visual_type)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);
  GdkVisual *return_val;
  int i;

  return_val = NULL;
  for (i = 0; i < riscos_screen->nvisuals; i++)
    if ((depth == riscos_screen->visuals[i]->depth) &&
	(visual_type == riscos_screen->visuals[i]->type))
      {
	return_val = (GdkVisual *) riscos_screen->visuals[i];
	break;
      }

  return return_val;
}

void
_gdk_riscos_screen_query_depths  (GdkScreen * screen,
				    gint **depths,
				    gint  *count)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);

  *count = riscos_screen->navailable_depths;
  *depths = riscos_screen->available_depths;
}

void
_gdk_riscos_screen_query_visual_types (GdkScreen * screen,
					 GdkVisualType **visual_types,
					 gint           *count)
{
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN (screen);

  *count = riscos_screen->navailable_types;
  *visual_types = riscos_screen->available_types;
}

GList *
_gdk_riscos_screen_list_visuals (GdkScreen *screen)
{
  GList *list;
  GdkRiscosScreen *riscos_screen;
  guint i;

  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);
  riscos_screen = GDK_RISCOS_SCREEN (screen);

  list = NULL;

  for (i = 0; i < riscos_screen->nvisuals; ++i)
    list = g_list_append (list, riscos_screen->visuals[i]);

  return list;
}

static void
gdk_visual_decompose_mask (gulong  mask,
			   gint   *shift,
			   gint   *prec)
{
  *shift = 0;
  *prec = 0;

  if (mask == 0)
    {
      g_warning ("Mask is 0 in visual. Server bug ?");
      return;
    }

  while (!(mask & 0x1))
    {
      (*shift)++;
      mask >>= 1;
    }

  while (mask & 0x1)
    {
      (*prec)++;
      mask >>= 1;
    }
}
