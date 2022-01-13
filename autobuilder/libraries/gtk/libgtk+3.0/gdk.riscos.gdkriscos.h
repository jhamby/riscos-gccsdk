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

#ifndef __GDK_RISCOS_H__
#define __GDK_RISCOS_H__

#include <gdk/gdk.h>
#include "oslib/wimp.h"
#include "oslib/toolbox.h"
#undef _
#undef NONE
#undef UNKNOWN

#define __GDKRISCOS_H_INSIDE__

#include <gdk/riscos/gdkriscosdisplaymanager.h>

#undef __GDKRISCOS_H_INSIDE__

G_BEGIN_DECLS

#define GDK_RISCOS_ANY_WIMP_EVENT ((wimp_event_no)-1)
#define GDK_RISCOS_ANY_WIMP_MESSAGE ((unsigned)-1)

typedef enum
{
  GDK_EVENT_HANDLER_MATCH_TYPE = 1 << 0,
  GDK_EVENT_HANDLER_MATCH_WINDOW = 1 << 1,
  GDK_EVENT_HANDLER_MATCH_FUNC = 1 << 2,
  GDK_EVENT_HANDLER_MATCH_DATA = 1 << 3,
} GDKEventHandlerMatchType;

typedef enum
{
  GDK_MESSAGE_HANDLER_MATCH_TYPE = 1 << 0,
  GDK_MESSAGE_HANDLER_MATCH_FUNC = 1 << 1,
  GDK_MESSAGE_HANDLER_MATCH_DATA = 1 << 2,
} GDKMessageHandlerMatchType;

typedef gboolean (*gdk_riscos_raw_event_handler)(wimp_event_no, wimp_block *, void *);
typedef gboolean (*gdk_riscos_message_handler)(wimp_event_no, wimp_block *, void *);

/* Add a handler function to be called when a WIMP event occurs on the
 * given window.
 * The event type can be GDK_RISCOS_ANY_WIMP_EVENT to call the handler on all
 * events.
 * The window_handle can be wimp_BACKGROUND to call the handler on all windows.
 * The handler function is called with a pointer to the event data and a
 * user supplied pointer as arguments.
 * The handler should return TRUE if it handled the event and does not wish
 * GDK to handle it or FALSE if it does wish GDK to handle it.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_add_raw_event_handler (wimp_event_no type,
				       wimp_w window_handle,
				       gdk_riscos_raw_event_handler handler_func,
				       void *handler_data);

/* Remove a WIMP event handler function.
 * The arguments given must match those that were given to
 * gdk_riscos_add_raw_event_handler in order to be successful.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_remove_raw_event_handler (wimp_event_no type,
					  wimp_w window_handle,
					  gdk_riscos_raw_event_handler handler_func,
					  void *handler_data);

/* Remove a WIMP event handler function that matches one or more of the
 * given arguments as defined by the mask.
 * Useful for removing handlers provided as lambda functions where the
 * function address is not known (ie, remove by data only).
 */
GDK_AVAILABLE_IN_ALL
int gdk_riscos_remove_raw_event_handler_matched(GDKEventHandlerMatchType mask,
						wimp_event_no type,
						wimp_w window_handle,
						gdk_riscos_raw_event_handler handler_func,
						void *handler_data);

/* Add a handler function to be called when a WIMP message is delivered.
 * The message type can be GDK_RISCOS_ANY_WIMP_MESSAGE to call the handler on all
 * messages.
 * The handler function is called with a pointer to the message event data and a
 * user supplied pointer as arguments.
 * The handler should return TRUE if it handled the message and does not wish
 * GDK to handle it or FALSE if it does wish GDK to handle it.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_add_message_handler (unsigned message_no,
				     gdk_riscos_message_handler handler_func,
				     void *data);

/* Remove a WIMP message handler function.
 * The arguments given must match those that were given to
 * gdk_riscos_add_message_handler in order to be successful.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_remove_message_handler (unsigned message_no,
					gdk_riscos_message_handler handler_func,
					void *data);

/* Remove a WIMP message handler function that matches one or more of the
 * given arguments as defined by the mask.
 * Useful for removing handlers provided as lambda functions where the
 * function address is not known (ie, remove by data only).
 */
GDK_AVAILABLE_IN_ALL
int gdk_riscos_remove_message_handler_matched (GDKMessageHandlerMatchType mask,
					       unsigned message_no,
					       gdk_riscos_message_handler handler_func,
					       void *data);

/* Return the WIMP handle of the given GDK window. */
GDK_AVAILABLE_IN_ALL
wimp_w gdk_riscos_window_get_handle (GdkWindow *gdk_window);

GDK_AVAILABLE_IN_ALL
guint gdk_riscos_display_get_user_time (GdkDisplay *display);

/* Call SWI Wimp_Poll with the user supplied block.
 * The handlers registered with gdk_riscos_add_raw_event_handler are
 * called first and if appropriate, the event is converted and
 * processed by GDK.
 */
GDK_AVAILABLE_IN_ALL
wimp_event_no gdk_riscos_event_poll (wimp_block *poll_block);

/* Call this before opening a display so that the SWI Toolbox_Initialise
 * is called instead of SWI Wimp_Initialise.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_enable_toolbox (const char *resource_dir_name,
				toolbox_block *tb_block,
				messagetrans_control_block *tb_messages);

/* Allows the caller to set whether the given window can be resized
 * by the user via shift drag. This isn't always desirable, for example,
 * if the window is to be nested.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_window_enable_user_resize(GdkWindow *gdk_window, gboolean enable);

/* Allows the caller to declare that this window will be nested within
 * another. This prevents GDK from reopening the window on a resize
 * which could mess up the windows nesting position.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_window_set_nested(GdkWindow *window);

GDK_AVAILABLE_IN_ALL
gboolean gdk_riscos_clipboard_is_text_available(GdkDisplay *display);

GDK_AVAILABLE_IN_ALL
gchar *gdk_riscos_clipboard_request_text(GdkDisplay *display);

GDK_AVAILABLE_IN_ALL
void gdk_riscos_selection_claim (GdkDisplay *display,
				 const void *data,
				 size_t size);
GDK_AVAILABLE_IN_ALL
void gdk_riscos_selection_release (GdkDisplay *display);

/* Place the given data on the clipboard.
 * If data is NULL or size is 0, the current GDK selection is used instead.
 */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_clipboard_claim(GdkDisplay *display,
				const void *data,
				size_t size);

/* Free resources used by the clipboard.  */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_clipboard_free(GdkDisplay *display);

/* Send the release entity wimp message for the clipboard.  */
GDK_AVAILABLE_IN_ALL
void gdk_riscos_clipboard_release(GdkDisplay *display);

G_END_DECLS

#endif /* __GDK_RISCOS_H__ */
