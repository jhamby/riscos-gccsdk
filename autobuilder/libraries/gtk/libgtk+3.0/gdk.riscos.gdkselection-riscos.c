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
#include "gdkprivate-riscos.h"
#include "gdkriscos.h"
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

  if (riscos_display->selection_window != owner)
    {
      riscos_display->selection_window = owner;

      wimp_message *message = &riscos_display->poll_block.message;
      message->size = sizeof(wimp_full_message_claim_entity);
      message->your_ref = 0;
      message->action = message_CLAIM_ENTITY;
      message->data.claim_entity.flags = wimp_CLAIM_CARET_OR_SELECTION;
      xwimp_send_message(wimp_USER_MESSAGE, message, wimp_BROADCAST);
    }

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

static gint
make_list (const gchar  *text,
           gint          length,
           gboolean      latin1,
           gchar      ***list)
{
  GSList *strings = NULL;
  gint n_strings = 0;
  gint i;
  const gchar *p = text;
  const gchar *q;
  GSList *tmp_list;
  GError *error = NULL;

  while (p < text + length)
    {
      gchar *str;

      q = p;
      while (*q && q < text + length)
        q++;

      if (latin1)
        {
          str = g_convert (p, q - p,
                           "UTF-8", "ISO-8859-1",
                           NULL, NULL, &error);

          if (!str)
            {
              g_warning ("Error converting selection from STRING: %s",
                         error->message);
              g_error_free (error);
            }
        }
      else
        {
          str = g_strndup (p, q - p);
          if (!g_utf8_validate (str, -1, NULL))
            {
              g_warning ("Error converting selection from UTF8_STRING");
              g_free (str);
              str = NULL;
            }
        }

      if (str)
        {
          strings = g_slist_prepend (strings, str);
          n_strings++;
        }

      p = q + 1;
    }

  if (list)
    {
      *list = g_new (gchar *, n_strings + 1);
      (*list)[n_strings] = NULL;
    }

  i = n_strings;
  tmp_list = strings;
  while (tmp_list)
    {
      if (list)
        (*list)[--i] = tmp_list->data;
      else
        g_free (tmp_list->data);

      tmp_list = tmp_list->next;
    }

  g_slist_free (strings);

  return n_strings;
}

gint
_gdk_riscos_display_text_property_to_utf8_list (GdkDisplay    *display,
						 GdkAtom        encoding,
						 gint           format,
						 const guchar  *text,
						 gint           length,
						 gchar       ***list)
{
  g_return_val_if_fail (text != NULL, 0);
  g_return_val_if_fail (length >= 0, 0);
  g_return_val_if_fail (GDK_IS_DISPLAY (display), 0);

  if (encoding == GDK_TARGET_STRING)
    {
      return make_list ((gchar *)text, length, TRUE, list);
    }
  else if (encoding == gdk_atom_intern_static_string ("UTF8_STRING"))
    {
      return make_list ((gchar *)text, length, FALSE, list);
    }

  if (list)
    *list = NULL;
  return 0;
}

gchar *
_gdk_riscos_display_utf8_to_string_target (GdkDisplay  *display,
					    const gchar *str)
{
  return NULL;
}

static gboolean
_gdk_riscos_realloc_selection (GdkRiscosSelection *selection, size_t size)
{
  if (!selection->buffer)
    {
      selection->buffer = malloc(size);
      if (!selection->buffer)
	return false;
      selection->max_size = size;
    }
  else
    {
      if (size > selection->max_size)
	{
	  void *new_buffer = realloc(selection->buffer, size);
	  if (!new_buffer)
	    return false;
	  selection->buffer = new_buffer;
	  selection->max_size = size;
	}
    }

  return true;
}

void gdk_riscos_selection_release (GdkDisplay *display)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  if (riscos_display->selection.claimed)
    {
      if (riscos_display->selection.buffer)
	free(riscos_display->selection.buffer);
      riscos_display->selection.buffer = NULL;
      riscos_display->selection.max_size = 0;
      riscos_display->selection.size = 0;
      riscos_display->selection.claimed = FALSE;
    }
}

void gdk_riscos_selection_claim (GdkDisplay *display, const void *data, size_t size)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  if (!size)
    {
      gdk_riscos_selection_release (display);
      return;
    }

  if (size > riscos_display->selection.max_size)
    if (!_gdk_riscos_realloc_selection(&riscos_display->selection, size))
      return;

  riscos_display->selection.size = size;
  memcpy(riscos_display->selection.buffer, data, size);

  if (!riscos_display->selection.claimed)
    {
      wimp_message *message = &riscos_display->poll_block.message;
      message->size = sizeof(wimp_full_message_claim_entity);
      message->your_ref = 0;
      message->action = message_CLAIM_ENTITY;
      message->data.claim_entity.flags = wimp_CLAIM_SELECTION;
      xwimp_send_message(wimp_USER_MESSAGE, message, wimp_BROADCAST);
      riscos_display->selection.claimed = TRUE;
    }
}
