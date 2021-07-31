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

#ifndef __GDK_DEVICE_RISCOS_H__
#define __GDK_DEVICE_RISCOS_H__

#include <gdk/gdkdeviceprivate.h>

G_BEGIN_DECLS

#define GDK_TYPE_RISCOS_DEVICE         (gdk_riscos_device_get_type ())
#define GDK_RISCOS_DEVICE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GDK_TYPE_RISCOS_DEVICE, GdkRISCOSDevice))
#define GDK_RISCOS_DEVICE_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GDK_TYPE_RISCOS_DEVICE, GdkRISCOSDeviceClass))
#define GDK_IS_RISCOS_DEVICE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GDK_TYPE_RISCOS_DEVICE))
#define GDK_IS_RISCOS_DEVICE_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GDK_TYPE_RISCOS_DEVICE))
#define GDK_RISCOS_DEVICE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GDK_TYPE_RISCOS_DEVICE, GdkRISCOSDeviceClass))

typedef struct _GdkRiscosDevice GdkRiscosDevice;
typedef struct _GdkRiscosDeviceClass GdkRiscosDeviceClass;

struct _GdkRiscosDevice
{
  GdkDevice parent_instance;
};

struct _GdkRiscosDeviceClass
{
  GdkDeviceClass parent_class;
};

G_GNUC_INTERNAL
GType gdk_riscos_device_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __GDK_DEVICE_RISCOS_H__ */
