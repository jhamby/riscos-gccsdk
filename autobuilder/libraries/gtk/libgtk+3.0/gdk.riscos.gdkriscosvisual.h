/* gdkriscosvisual.h
 *
 * Copyright (C) 2011  Alexander Larsson  <alexl@redhat.com>
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

#ifndef __GDK_RISCOS_VISUAL_H__
#define __GDK_RISCOS_VISUAL_H__

#include <gdk/gdk.h>

G_BEGIN_DECLS

#define GDK_TYPE_RISCOS_VISUAL              (gdk_riscos_visual_get_type ())
#define GDK_RISCOS_VISUAL(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_RISCOS_VISUAL, GdkRiscosVisual))
#define GDK_RISCOS_VISUAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_RISCOS_VISUAL, GdkRiscosVisualClass))
#define GDK_IS_RISCOS_VISUAL(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_RISCOS_VISUAL))
#define GDK_IS_RISCOS_VISUAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_RISCOS_VISUAL))
#define GDK_RISCOS_VISUAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_RISCOS_VISUAL, GdkRiscosVisualClass))

#ifdef GDK_COMPILATION
typedef struct _GdkRiscosVisual GdkRiscosVisual;
#else
typedef GdkVisual GdkRiscosVisual;
#endif
typedef struct _GdkRiscosVisualClass GdkRiscosVisualClass;


GType gdk_riscos_visual_get_type (void);

G_END_DECLS

#endif /* __GDK_RISCOS_VISUAL_H__ */
