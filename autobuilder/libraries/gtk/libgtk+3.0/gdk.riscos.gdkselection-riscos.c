/*
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

#include "gdkdisplay-riscos.h"
#include "gdkselection.h"
#include "gdkproperty.h"
#include "gdkprivate.h"
#include <oslib/wimp.h>

#include <string.h>

typedef struct _OwnerInfo OwnerInfo;

struct _OwnerInfo
{
  GdkAtom    selection;
  GdkWindow *owner;
  gulong     serial;
};

static GSList *owner_list;

GdkWindow *
_gdk_riscos_display_get_selection_owner (GdkDisplay *display,
					  GdkAtom     selection)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);
  GSList *tmp_list;
  OwnerInfo *info;

  if (gdk_display_is_closed (display))
    return NULL;

  tmp_list = owner_list;
  while (tmp_list)
    {
      info = tmp_list->data;
      if (info->selection == selection)
	return info->owner;
      tmp_list = tmp_list->next;
    }

  riscos_display->selection_window = NULL;

  return NULL;
}

gboolean
_gdk_riscos_display_set_selection_owner (GdkDisplay *display,
					  GdkWindow  *owner,
					  GdkAtom     selection,
					  guint32     time,
					  gboolean    send_event)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);
  GSList *tmp_list;
  OwnerInfo *info;

  if (gdk_display_is_closed (display))
    return FALSE;

  tmp_list = owner_list;
  while (tmp_list)
    {
      info = tmp_list->data;
      if (info->selection == selection)
        {
          owner_list = g_slist_remove (owner_list, info);
          g_free (info);
          break;
        }
      tmp_list = tmp_list->next;
    }

  if (owner)
    {
      info = g_new (OwnerInfo, 1);
      info->owner = owner;
      info->serial = _gdk_display_get_next_serial (display);

      info->selection = selection;

      owner_list = g_slist_prepend (owner_list, info);
    }

  riscos_display->selection_window = owner;

  return TRUE;
}

void
_gdk_riscos_display_send_selection_notify (GdkDisplay *dispay,
					    GdkWindow        *requestor,
					    GdkAtom          selection,
					    GdkAtom          target,
					    GdkAtom          property,
					    guint32          time)
{
}

gint
_gdk_riscos_display_get_selection_property (GdkDisplay  *display,
					     GdkWindow   *requestor,
					     guchar     **data,
					     GdkAtom     *ret_type,
					     gint        *ret_format)
{
  return 0;
}

void
_gdk_riscos_display_convert_selection (GdkDisplay *display,
					GdkWindow  *requestor,
					GdkAtom     selection,
					GdkAtom     target,
					guint32     time)
{
}

gint
_gdk_riscos_display_text_property_to_utf8_list (GdkDisplay    *display,
						 GdkAtom        encoding,
						 gint           format,
						 const guchar  *text,
						 gint           length,
						 gchar       ***list)
{
  return 0;
}

gchar *
_gdk_riscos_display_utf8_to_string_target (GdkDisplay  *display,
					    const gchar *str)
{
  return NULL;
}
