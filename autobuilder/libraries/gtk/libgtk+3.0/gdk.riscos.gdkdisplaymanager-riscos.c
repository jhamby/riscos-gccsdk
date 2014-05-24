/* GDK - The GIMP Drawing Kit
 * gdkdisplaymanager-riscos.c
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

/*
 * Written by Lee Noar using code and guidance from other targets.
 */

#include "config.h"

#include "gdkdisplay-riscos.h"
#include "gdkriscosdisplaymanager.h"
#include "gdkprivate-riscos.h"

#include "gdkdisplaymanagerprivate.h"
#include "gdkinternals.h"

struct _GdkRiscosDisplayManager
{
  GdkDisplayManager parent;

  GdkDisplay *default_display;
  GSList *displays;
};

G_DEFINE_TYPE (GdkRiscosDisplayManager, gdk_riscos_display_manager, GDK_TYPE_DISPLAY_MANAGER)

static GdkDisplay *
gdk_riscos_display_manager_open_display (GdkDisplayManager *manager,
                                         const gchar       *name)
{
  return _gdk_riscos_display_open (name);
}

static GSList *
gdk_riscos_display_manager_list_displays (GdkDisplayManager *manager)
{
  GdkRiscosDisplayManager *manager_riscos = GDK_RISCOS_DISPLAY_MANAGER (manager);

  return g_slist_copy (manager_riscos->displays);
}

static GdkDisplay *
gdk_riscos_display_manager_get_default_display (GdkDisplayManager *manager)
{
  return GDK_RISCOS_DISPLAY_MANAGER (manager)->default_display;
}

static void
gdk_riscos_display_manager_set_default_display (GdkDisplayManager *manager,
                                                GdkDisplay        *display)
{
  GdkRiscosDisplayManager *manager_riscos = GDK_RISCOS_DISPLAY_MANAGER (manager);

  manager_riscos->default_display = display;
}

#include "../gdkkeynames.c"

static gchar *
gdk_riscos_display_manager_get_keyval_name (GdkDisplayManager *manager,
                                            guint              keyval)
{
  return _gdk_keyval_name (keyval);
}

static guint
gdk_riscos_display_manager_lookup_keyval (GdkDisplayManager *manager,
                                          const gchar       *name)
{
  return _gdk_keyval_from_name (name);
}

static void
gdk_riscos_display_manager_init (GdkRiscosDisplayManager *manager)
{
//  _gdk_riscos_windowing_init ();
}

static void
gdk_riscos_display_manager_finalize (GObject *object)
{
  g_error ("A GdkRISCOSDisplayManager object was finalized. This should not happen");
  G_OBJECT_CLASS (gdk_riscos_display_manager_parent_class)->finalize (object);
}

static void
gdk_riscos_display_manager_class_init (GdkRiscosDisplayManagerClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GdkDisplayManagerClass *manager_class = GDK_DISPLAY_MANAGER_CLASS (class);

  object_class->finalize = gdk_riscos_display_manager_finalize;

  manager_class->open_display = gdk_riscos_display_manager_open_display;
  manager_class->list_displays = gdk_riscos_display_manager_list_displays;
  manager_class->set_default_display = gdk_riscos_display_manager_set_default_display;
  manager_class->get_default_display = gdk_riscos_display_manager_get_default_display;
  manager_class->atom_intern = _gdk_riscos_display_manager_atom_intern;
  manager_class->get_atom_name = _gdk_riscos_display_manager_get_atom_name;
  manager_class->lookup_keyval = gdk_riscos_display_manager_lookup_keyval;
  manager_class->get_keyval_name = gdk_riscos_display_manager_get_keyval_name;
}

void
_gdk_riscos_display_manager_add_display (GdkDisplayManager *manager,
					 GdkDisplay        *display)
{
  GdkRiscosDisplayManager *manager_riscos = GDK_RISCOS_DISPLAY_MANAGER (manager);

  if (manager_riscos->displays == NULL)
    gdk_display_manager_set_default_display (manager, display);

  manager_riscos->displays = g_slist_prepend (manager_riscos->displays, display);
}

void
_gdk_riscos_display_manager_remove_display (GdkDisplayManager *manager,
					    GdkDisplay        *display)
{
  GdkRiscosDisplayManager *manager_riscos = GDK_RISCOS_DISPLAY_MANAGER (manager);

  manager_riscos->displays = g_slist_remove (manager_riscos->displays, display);

  if (manager_riscos->default_display == display)
    {
      if (manager_riscos->displays)
        gdk_display_manager_set_default_display (manager, manager_riscos->displays->data);
      else
        gdk_display_manager_set_default_display (manager, NULL);
    }
}
