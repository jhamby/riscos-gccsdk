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

#ifndef __GDK_RISCOS_H__
#define __GDK_RISCOS_H__

#include <gdk/gdk.h>
#include "oslib/wimp.h"
#undef _
#undef NONE
#undef UNKNOWN

#define __GDKRISCOS_H_INSIDE__

#include <gdk/riscos/gdkriscosdisplaymanager.h>

#undef __GDKRISCOS_H_INSIDE__

G_BEGIN_DECLS

typedef gboolean (*raw_event_handler)(wimp_event_no, wimp_block *);

void
gdk_riscos_add_raw_event_handler (wimp_event_no type,
				  wimp_w window_handle,
				  raw_event_handler handler_func);

wimp_w
gdk_riscos_window_get_handle (GdkWindow *gdk_window);

guint
gdk_riscos_display_get_user_time (GdkDisplay *display);

G_END_DECLS

#endif /* __GDK_RISCOS_H__ */
