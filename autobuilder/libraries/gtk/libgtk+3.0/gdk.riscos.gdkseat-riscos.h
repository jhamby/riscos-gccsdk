/* GDK - The GIMP Drawing Kit
 * Copyright (C) 2015 Red Hat
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
 *
 * Author: Carlos Garnacho <carlosg@gnome.org>
 */

#ifndef __GDK_RISCOS_SEAT_H__
#define __GDK_RISCOS_SEAT_H__

#include "config.h"

#include <gdk/gdkseatprivate.h>

#define GDK_TYPE_RISCOS_SEAT         (gdk_riscos_seat_get_type ())
#define GDK_RISCOS_SEAT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GDK_TYPE_RISCOS_SEAT, GdkRiscosSeat))
#define GDK_RISCOS_SEAT_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST ((c), GDK_TYPE_RISCOS_SEAT, GdkRiscosSeatClass))
#define GDK_IS_RISCOS_SEAT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GDK_TYPE_RISCOS_SEAT))
#define GDK_IS_RISCOS_SEAT_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE ((c), GDK_TYPE_RISCOS_SEAT))
#define GDK_RISCOS_SEAT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GDK_TYPE_RISCOS_SEAT, GdkRiscosSeatClass))

typedef struct _GdkRiscosSeat GdkRiscosSeat;
typedef struct _GdkRiscosSeatClass GdkRiscosSeatClass;

struct _GdkRiscosSeatClass
{
  GdkSeatClass parent_class;
};
struct _GdkRiscosSeat
{
  GdkSeat parent_instance;

  GdkDisplay *display;
  GdkDeviceManager *device_manager;

  GdkDevice *master_pointer;
  GdkDevice *master_keyboard;

  GdkWindow *pointer_grab;
  GdkWindow *keyboard_grab;
};

GType gdk_riscos_seat_get_type (void) G_GNUC_CONST;

#endif /* __GDK_RISCOS_SEAT_H__ */
