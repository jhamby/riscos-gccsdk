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

#include "gdkproperty.h"

#include "gdkmain.h"
#include "gdkprivate.h"
#include "gdkinternals.h"
#include "gdkdisplay-riscos.h"
#include "gdkscreen-riscos.h"
#include "gdkselection.h"
#include "gdkprivate-riscos.h"

#include <string.h>

GdkAtom
_gdk_riscos_display_manager_atom_intern (GdkDisplayManager *manager,
					 const gchar *atom_name, 
					 gboolean     only_if_exists)
{
  return _GDK_MAKE_ATOM (g_quark_from_string (atom_name));
}

GdkAtom
_gdk_riscos_display_manager_atom_intern_static_string (GdkDisplayManager *manager,
						       const gchar *atom_name)
{
  return _GDK_MAKE_ATOM (g_quark_from_static_string (atom_name));
}

static const char *
get_atom_name (GdkAtom atom)
{
  return g_quark_to_string (GPOINTER_TO_UINT(atom));
}

gchar *
_gdk_riscos_display_manager_get_atom_name (GdkDisplayManager *manager,
					   GdkAtom atom)
{
  return g_strdup (get_atom_name (atom));
}

gboolean
_gdk_riscos_window_get_property (GdkWindow   *window,
				 GdkAtom      property,
				 GdkAtom      type,
				 gulong       offset,
				 gulong       length,
				 gint         pdelete,
				 GdkAtom     *actual_property_type,
				 gint        *actual_format_type,
				 gint        *actual_length,
				 guchar     **data)
{
  return FALSE;
}

void
_gdk_riscos_window_change_property (GdkWindow    *window,
				    GdkAtom       property,
				    GdkAtom       type,
				    gint          format,
				    GdkPropMode   mode,
				    const guchar *data,
				    gint          nelements)
{
  g_return_if_fail (!window || GDK_WINDOW_IS_RISCOS (window));
}

void
_gdk_riscos_window_delete_property (GdkWindow *window,
				    GdkAtom    property)
{
  g_return_if_fail (!window || GDK_WINDOW_IS_RISCOS (window));
}
