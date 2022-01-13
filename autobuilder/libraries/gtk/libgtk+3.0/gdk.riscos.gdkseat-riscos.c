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

#include "config.h"

#include "gdkseat-riscos.h"
#include "gdkwindow-riscos.h"

G_DEFINE_TYPE (GdkRiscosSeat, gdk_riscos_seat, GDK_TYPE_SEAT)

static void
gdk_riscos_seat_finalize (GObject *object)
{
}

static GdkSeatCapabilities
gdk_riscos_seat_get_capabilities (GdkSeat *seat)
{
  GdkRiscosSeat *riscos_seat = GDK_RISCOS_SEAT (seat);
  GdkSeatCapabilities caps = 0;

  if (riscos_seat->master_pointer)
    caps |= GDK_SEAT_CAPABILITY_POINTER;
  if (riscos_seat->master_keyboard)
    caps |= GDK_SEAT_CAPABILITY_KEYBOARD;

  return caps;
}

/*static void
gdk_riscos_seat_set_grab_window (GdkRiscosSeat *seat,
				 GdkWindow      *window)
{
}*/

static GdkGrabStatus
gdk_riscos_seat_grab (GdkSeat                *seat,
		      GdkWindow              *window,
		      GdkSeatCapabilities     capabilities,
		      gboolean                owner_events,
		      GdkCursor              *cursor,
		      const GdkEvent         *event,
		      GdkSeatGrabPrepareFunc  prepare_func,
		      gpointer                prepare_func_data)
{
  GdkRiscosSeat *riscos_seat = GDK_RISCOS_SEAT(seat);

  if (capabilities == GDK_SEAT_CAPABILITY_POINTER)
    riscos_seat->pointer_grab = window;
  else if (capabilities == GDK_SEAT_CAPABILITY_KEYBOARD)
    riscos_seat->keyboard_grab = window;

  return GDK_GRAB_SUCCESS;
}

static void
gdk_riscos_seat_ungrab (GdkSeat *seat)
{
  GdkRiscosSeat *riscos_seat = GDK_RISCOS_SEAT(seat);

  riscos_seat->pointer_grab = NULL;
  riscos_seat->keyboard_grab = NULL;
}

static GdkDevice *
gdk_riscos_seat_get_master (GdkSeat             *seat,
			    GdkSeatCapabilities  capabilities)
{
  GdkRiscosSeat *riscos_seat = GDK_RISCOS_SEAT (seat);

  if (capabilities == GDK_SEAT_CAPABILITY_POINTER)
    return riscos_seat->master_pointer;
  else if (capabilities == GDK_SEAT_CAPABILITY_KEYBOARD)
    return riscos_seat->master_keyboard;

  return NULL;
}

static GList *
gdk_riscos_seat_get_slaves (GdkSeat             *seat,
                             GdkSeatCapabilities  capabilities)
{
  return NULL;
}

static void
gdk_riscos_seat_class_init (GdkRiscosSeatClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GdkSeatClass *seat_class = GDK_SEAT_CLASS (klass);

  object_class->finalize = gdk_riscos_seat_finalize;

  seat_class->get_capabilities = gdk_riscos_seat_get_capabilities;
  seat_class->grab = gdk_riscos_seat_grab;
  seat_class->ungrab = gdk_riscos_seat_ungrab;
  seat_class->get_master = gdk_riscos_seat_get_master;
  seat_class->get_slaves = gdk_riscos_seat_get_slaves;
}

static void
gdk_riscos_seat_init (GdkRiscosSeat *seat)
{
}
