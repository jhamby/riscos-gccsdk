/*
 * Copyright © 2016 Red Hat, Inc
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

#include "config.h"

#include <glib.h>
#include <gio/gio.h>

#include "gdkmonitor-riscos.h"
#include "gdkscreen-riscos.h"


G_DEFINE_TYPE (GdkRiscosMonitor, gdk_riscos_monitor, GDK_TYPE_MONITOR)

static void
gdk_riscos_monitor_init (GdkRiscosMonitor *monitor)
{
}

static void
gdk_riscos_monitor_get_workarea (GdkMonitor   *monitor,
				 GdkRectangle *dest)
{
  GdkScreen *screen = gdk_display_get_default_screen (monitor->display);
  GdkRiscosScreen *riscos_screen = GDK_RISCOS_SCREEN(screen);

  dest->x = 0;
  dest->y = 0;
  dest->width = riscos_screen->width;
  dest->height = riscos_screen->height;

  /* TODO: Find height of Iconbar and reduce screen height.  */
}

static void
gdk_riscos_monitor_class_init (GdkRiscosMonitorClass *class)
{
  GDK_MONITOR_CLASS (class)->get_workarea = gdk_riscos_monitor_get_workarea;
}
