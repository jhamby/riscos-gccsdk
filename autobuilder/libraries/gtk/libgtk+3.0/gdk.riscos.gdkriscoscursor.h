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

#if !defined (__GDKRISCOS_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gdk/gdkriscos.h> can be included directly."
#endif

#ifndef __GDK_RISCOS_CURSOR_H__
#define __GDK_RISCOS_CURSOR_H__

#include <gdk/gdk.h>

G_BEGIN_DECLS

#define GDK_TYPE_RISCOS_CURSOR              (gdk_riscos_cursor_get_type ())
#define GDK_RISCOS_CURSOR(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_RISCOS_CURSOR, GdkRiscosCursor))
#define GDK_RISCOS_CURSOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_RISCOS_CURSOR, GdkRiscosCursorClass))
#define GDK_IS_RISCOS_CURSOR(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_RISCOS_CURSOR))
#define GDK_IS_RISCOS_CURSOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_RISCOS_CURSOR))
#define GDK_RISCOS_CURSOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_RISCOS_CURSOR, GdkRiscosCursorClass))

#ifdef GDK_COMPILATION
typedef struct _GdkRiscosCursor GdkRiscosCursor;
#else
typedef GdkCursor GdkRiscosCursor;
#endif
typedef struct _GdkRiscosCursorClass GdkRiscosCursorClass;

GType    gdk_riscos_cursor_get_type          (void);

G_END_DECLS

#endif /* __GDK_RISCOS_CURSOR_H__ */
