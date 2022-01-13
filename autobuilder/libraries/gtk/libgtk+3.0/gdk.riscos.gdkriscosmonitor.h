/*
 * gdkriscosmonitor.h adapted from gdkbroadwaymonitor.h
 *
 * Copyright 2016 Red Hat, Inc.
 *
 * Matthias Clasen <mclasen@redhat.com>
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

#ifndef __GDK_RISCOS_MONITOR_H__
#define __GDK_RISCOS_MONITOR_H__

#if !defined (__RISCOS_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gdk/gdkriscos.h> can be included directly."
#endif

#include <gdk/gdkmonitor.h>

G_BEGIN_DECLS

#define GDK_TYPE_RISCOS_MONITOR           (gdk_riscos_monitor_get_type ())
#define GDK_RISCOS_MONITOR(object)        (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_RISCOS_MONITOR, GdkRiscosMonitor))
#define GDK_IS_RISCOS_MONITOR(object)     (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_RISCOS_MONITOR))

typedef struct _GdkRiscosMonitor      GdkRiscosMonitor;
typedef struct _GdkRiscosMonitorClass GdkRiscosMonitorClass;

GDK_AVAILABLE_IN_3_22
GType             gdk_riscos_monitor_get_type            (void) G_GNUC_CONST;

G_END_DECLS

#endif  /* __GDK_RISCOS_MONITOR_H__ */
