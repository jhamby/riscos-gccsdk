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

#ifndef __GDK_DEVICE_MANAGER_RISCOS_H__
#define __GDK_DEVICE_MANAGER_RISCOS_H__

#include <gdk/gdkdevicemanagerprivate.h>

G_BEGIN_DECLS

#define GDK_TYPE_RISCOS_DEVICE_MANAGER         (gdk_riscos_device_manager_get_type ())
#define GDK_RISCOS_DEVICE_MANAGER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GDK_TYPE_RISCOS_DEVICE_MANAGER, GdkRISCOSDeviceManager))
#define GDK_RISCOS_DEVICE_MANAGER_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GDK_TYPE_RISCOS_DEVICE_MANAGER, GdkRISCOSDeviceManagerClass))
#define GDK_IS_RISCOS_DEVICE_MANAGER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GDK_TYPE_RISCOS_DEVICE_MANAGER))
#define GDK_IS_RISCOS_DEVICE_MANAGER_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GDK_TYPE_RISCOS_DEVICE_MANAGER))
#define GDK_RISCOS_DEVICE_MANAGER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GDK_TYPE_RISCOS_DEVICE_MANAGER, GdkRISCOSDeviceManagerClass))

typedef struct _GdkRISCOSDeviceManager GdkRISCOSDeviceManager;
typedef struct _GdkRISCOSDeviceManagerClass GdkRISCOSDeviceManagerClass;

struct _GdkRISCOSDeviceManager
{
  GdkDeviceManager parent_object;
  GdkDevice *core_pointer;
  GdkDevice *core_keyboard;
};

struct _GdkRISCOSDeviceManagerClass
{
  GdkDeviceManagerClass parent_class;
};

GType gdk_riscos_device_manager_get_type (void) G_GNUC_CONST;
GdkDeviceManager *_gdk_riscos_device_manager_new (GdkDisplay *display);

G_END_DECLS

#endif /* __GDK_DEVICE_MANAGER_RISCOS_H__ */
