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

#include "gdkdisplay-riscos.h"
#include "gdkriscos.h"
#include "gdkscreen-riscos.h"
#include "gdkriscoswindow.h"
#include "gdkwindow-riscos.h"
#include "gdkprivate-riscos.h"
#include "gdkdevicemanager-riscos.h"
#include "gdkseat-riscos.h"

#include "oslib/wimp.h"
#include "oslib/osspriteop.h"
#include "oslib/os.h"
#include "oslib/report.h"
#include "oslib/osbyte.h"

#include "cairo-riscos.h"

//#define G_MAINLOOP_POLL
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <swis.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define INLINE_EVENT_HANDLERS 0
#if INLINE_EVENT_HANDLERS
#define INLINE_EVENT_HANDLER
#else
#define INLINE_EVENT_HANDLER __attribute__((noinline))
#endif

#define DEBUG_REDRAW 0

typedef struct _raw_event_handler_data
{
  wimp_event_no type;
  wimp_w window_handle;
  gdk_riscos_raw_event_handler handler_func;
  void *handler_data;

} raw_event_handler_data;

typedef struct _message_handler_data
{
  unsigned message_no;
  gdk_riscos_message_handler handler_func;
  void *data;

} message_handler_data;

typedef struct _GdkEventSource
{
  GSource source;

  GdkDisplay *display;
} GdkEventSource;

struct keymapping
{
    int riscos_key_code;
    guint gdk_key_code;
};

struct internal_mapping
{
  guint wimp_code;
  guint internal_code;
};

/* We use a hashmap at runtime for speed, but it still needs to be initialised from
 * something like the following.
 * TODO: check some of these codes at runtime.
 */
static const struct internal_mapping internal_keymap[] = {
  { 8, 47 },
  { 127, 89 },
  { ' ', 98 },
  { '!', 48 },
  { '"', 49 },
  { '#', 45 },
  { '$', 18 },
  { '%', 19 },
  { '^', 24 },
  { '&', 36 },
  { '\'', 79 },
  { '(', 38 },
  { ')', 39 },
  { '*', 21 },
  { ',', 102 },
  { '-', 23 },
  { '.', 104 },
  { '/', 39 },
  { '0', 48 },
  { '1', 49 },
  { '2', 17 },
  { '3', 18 },
  { '4', 19 },
  { '5', 52 },
  { '6', 36 },
  { '7', 21 },
  { '8', 38 },
  { '9', 39 },
  { ':', 87 },
  { ';', 87 },
  { '<', 102 },
  { '=', 93 },
  { '>', 103 },
  { '?', 104 },
  { '@', 79 },
  { 'A', 65 },
  { 'B', 100 },
  { 'C', 82 },
  { 'D', 50 },
  { 'E', 34 },
  { 'F', 67 },
  { 'G', 83 },
  { 'H', 84 },
  { 'I', 37 },
  { 'J', 69 },
  { 'K', 70 },
  { 'L', 86 },
  { 'M', 101 },
  { 'N', 85 },
  { 'O', 54 },
  { 'P', 55 },
  { 'Q', 16 },
  { 'R', 51 },
  { 'S', 81 },
  { 'T', 35 },
  { 'U', 53 },
  { 'V', 99 },
  { 'W', 33 },
  { 'X', 66 },
  { 'Y', 68 },
  { 'Z', 97 },
  { '[', 56 },
  { ']', 88 },
  { '\\', 120 },
  { '_', 23 },
  { '`', 45 },
  { 27, 112 },
  { 'a', 65 },
  { 'b', 100 },
  { 'c', 82 },
  { 'd', 50 },
  { 'e', 34 },
  { 'f', 67 },
  { 'g', 83 },
  { 'h', 84 },
  { 'i', 37 },
  { 'j', 69 },
  { 'k', 70 },
  { 'l', 86 },
  { 'm', 101 },
  { 'n', 85 },
  { 'o', 54 },
  { 'p', 55 },
  { 'q', 16 },
  { 'r', 51 },
  { 's', 81 },
  { 't', 35 },
  { 'u', 53 },
  { 'v', 99 },
  { 'w', 33 },
  { 'x', 66 },
  { 'y', 68 },
  { 'z', 97 },
  { '{', 56 },
  { '}', 88 },
  { '|', 94 },
  { '~', 120 }
};

static const struct keymapping function_keymap[] = {
    { wimp_KEY_LOGO, GDK_KEY_Meta_L },
    { wimp_KEY_MENU, GDK_KEY_Menu },
    { wimp_KEY_BACKSPACE, GDK_KEY_BackSpace },
    { wimp_KEY_RETURN, GDK_KEY_Return },
    { wimp_KEY_ESCAPE, GDK_KEY_Escape },
    { wimp_KEY_HOME, GDK_KEY_Home },
    { wimp_KEY_DELETE, GDK_KEY_Delete },
    { wimp_KEY_PRINT, GDK_KEY_Print },
    { wimp_KEY_PAGE_DOWN, GDK_KEY_Page_Down },
    { wimp_KEY_PAGE_UP, GDK_KEY_Page_Up },
    { wimp_KEY_F1, GDK_KEY_F1 },
    { wimp_KEY_F2, GDK_KEY_F2 },
    { wimp_KEY_F3, GDK_KEY_F3 },
    { wimp_KEY_F4, GDK_KEY_F4 },
    { wimp_KEY_F5, GDK_KEY_F5 },
    { wimp_KEY_F6, GDK_KEY_F6 },
    { wimp_KEY_F7, GDK_KEY_F7 },
    { wimp_KEY_F8, GDK_KEY_F8 },
    { wimp_KEY_F9, GDK_KEY_F9 },
    { wimp_KEY_TAB, GDK_KEY_Tab },
    { wimp_KEY_COPY, GDK_KEY_End },
    { wimp_KEY_LEFT, GDK_KEY_Left },
    { wimp_KEY_RIGHT, GDK_KEY_Right },
    { wimp_KEY_DOWN, GDK_KEY_Down },
    { wimp_KEY_UP, GDK_KEY_Up },
    { wimp_KEY_F10, GDK_KEY_F10 },
    { wimp_KEY_F11, GDK_KEY_F11 },
    { wimp_KEY_F12, GDK_KEY_F12 },
    { wimp_KEY_INSERT, GDK_KEY_Insert },
    { wimp_KEY_PRINT | wimp_KEY_SHIFT, GDK_KEY_Print },
    { wimp_KEY_F1 | wimp_KEY_SHIFT, GDK_KEY_F1 },
    { wimp_KEY_F2 | wimp_KEY_SHIFT, GDK_KEY_F2 },
    { wimp_KEY_F3 | wimp_KEY_SHIFT, GDK_KEY_F3 },
    { wimp_KEY_F4 | wimp_KEY_SHIFT, GDK_KEY_F4 },
    { wimp_KEY_F5 | wimp_KEY_SHIFT, GDK_KEY_F5 },
    { wimp_KEY_F6 | wimp_KEY_SHIFT, GDK_KEY_F6 },
    { wimp_KEY_F7 | wimp_KEY_SHIFT, GDK_KEY_F7 },
    { wimp_KEY_F8 | wimp_KEY_SHIFT, GDK_KEY_F8 },
    { wimp_KEY_F9 | wimp_KEY_SHIFT, GDK_KEY_F9 },
    { wimp_KEY_TAB | wimp_KEY_SHIFT, GDK_KEY_Tab },
    { wimp_KEY_COPY | wimp_KEY_SHIFT, GDK_KEY_End },
    { wimp_KEY_LEFT | wimp_KEY_SHIFT, GDK_KEY_Left },
    { wimp_KEY_RIGHT | wimp_KEY_SHIFT, GDK_KEY_Right },
    { wimp_KEY_DOWN | wimp_KEY_SHIFT, GDK_KEY_Page_Down },
    { wimp_KEY_UP | wimp_KEY_SHIFT, GDK_KEY_Page_Up },
    { wimp_KEY_F10 | wimp_KEY_SHIFT, GDK_KEY_F10 },
    { wimp_KEY_F11 | wimp_KEY_SHIFT, GDK_KEY_F11 },
    { wimp_KEY_F12 | wimp_KEY_SHIFT, GDK_KEY_F12 },
    { wimp_KEY_INSERT | wimp_KEY_SHIFT, GDK_KEY_Insert },
    { wimp_KEY_PRINT | wimp_KEY_CONTROL, GDK_KEY_Print },
    { wimp_KEY_F1 | wimp_KEY_CONTROL, GDK_KEY_F1 },
    { wimp_KEY_F2 | wimp_KEY_CONTROL, GDK_KEY_F2 },
    { wimp_KEY_F3 | wimp_KEY_CONTROL, GDK_KEY_F3 },
    { wimp_KEY_F4 | wimp_KEY_CONTROL, GDK_KEY_F4 },
    { wimp_KEY_F5 | wimp_KEY_CONTROL, GDK_KEY_F5 },
    { wimp_KEY_F6 | wimp_KEY_CONTROL, GDK_KEY_F6 },
    { wimp_KEY_F7 | wimp_KEY_CONTROL, GDK_KEY_F7 },
    { wimp_KEY_F8 | wimp_KEY_CONTROL, GDK_KEY_F8 },
    { wimp_KEY_F9 | wimp_KEY_CONTROL, GDK_KEY_F9 },
    { wimp_KEY_TAB | wimp_KEY_CONTROL, GDK_KEY_Tab },
    { wimp_KEY_COPY | wimp_KEY_CONTROL, GDK_KEY_End },
    { wimp_KEY_LEFT | wimp_KEY_CONTROL, GDK_KEY_Left },
    { wimp_KEY_RIGHT | wimp_KEY_CONTROL, GDK_KEY_Right },
    { wimp_KEY_DOWN | wimp_KEY_CONTROL, GDK_KEY_Down },
    { wimp_KEY_UP | wimp_KEY_CONTROL, GDK_KEY_Up },
    { wimp_KEY_F10 | wimp_KEY_CONTROL, GDK_KEY_F10 },
    { wimp_KEY_F11 | wimp_KEY_CONTROL, GDK_KEY_F11 },
    { wimp_KEY_F12 | wimp_KEY_CONTROL, GDK_KEY_F12 },
    { wimp_KEY_INSERT | wimp_KEY_CONTROL, GDK_KEY_Insert },
    { wimp_KEY_PRINT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_Print },
    { wimp_KEY_F1 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F1 },
    { wimp_KEY_F2 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F2 },
    { wimp_KEY_F3 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F3 },
    { wimp_KEY_F4 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F4 },
    { wimp_KEY_F5 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F5 },
    { wimp_KEY_F6 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F6 },
    { wimp_KEY_F7 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F7 },
    { wimp_KEY_F8 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F8 },
    { wimp_KEY_F9 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F9 },
    { wimp_KEY_TAB | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_Tab },
    { wimp_KEY_COPY | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_End },
    { wimp_KEY_LEFT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_Left },
    { wimp_KEY_RIGHT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_Right },
    { wimp_KEY_DOWN | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_Down },
    { wimp_KEY_UP | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_Up },
    { wimp_KEY_F10 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F10 },
    { wimp_KEY_F11 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F11 },
    { wimp_KEY_F12 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_F12 },
    { wimp_KEY_INSERT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, GDK_KEY_Insert }
};

extern void __pthread_disable_ints(void);
extern void __pthread_enable_ints(void);

void
gdk_riscos_add_raw_event_handler (wimp_event_no type,
				  wimp_w window_handle,
				  gdk_riscos_raw_event_handler handler_func,
				  void *handler_data)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);
  raw_event_handler_data *event;

  event = g_new (raw_event_handler_data, 1);
  event->type = type;
  event->window_handle = window_handle;
  event->handler_func = handler_func;
  event->handler_data = handler_data;

  riscos_display->raw_event_handlers = g_list_append (riscos_display->raw_event_handlers,
						      event);
}

void
gdk_riscos_remove_raw_event_handler (wimp_event_no type,
				     wimp_w window_handle,
				     gdk_riscos_raw_event_handler handler_func,
				     void *handler_data)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);
  GList *list = riscos_display->raw_event_handlers;
  GList *node = list;

  while (node)
    {
      GList *next = g_list_next(node);
      raw_event_handler_data *handler = node->data;
      if (handler->type == type &&
	  handler->window_handle == window_handle &&
	  handler->handler_func == handler_func &&
	  handler->handler_data == handler_data)
	{
	  g_free(handler);
	  list = g_list_delete_link(list, node);
	}
      node = next;
    }

  riscos_display->raw_event_handlers = list;
}

int
gdk_riscos_remove_raw_event_handler_matched(GDKEventHandlerMatchType mask,
					    wimp_event_no type,
					    wimp_w window_handle,
					    gdk_riscos_raw_event_handler handler_func,
					    void *handler_data)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);
  GList *list = riscos_display->raw_event_handlers;
  GList *node = list;
  int count = 0;
  int target = __builtin_popcount((unsigned)mask);

  while (node)
    {
      GList *next = g_list_next(node);
      raw_event_handler_data *handler = node->data;
      int match = 0;

      if ((mask & GDK_EVENT_HANDLER_MATCH_TYPE) && handler->type == type)
	match++;
      if ((mask & GDK_EVENT_HANDLER_MATCH_WINDOW) && handler->window_handle == window_handle)
	match++;
      if ((mask & GDK_EVENT_HANDLER_MATCH_FUNC) && handler->handler_func == handler_func)
	match++;
      if ((mask & GDK_EVENT_HANDLER_MATCH_DATA) && handler->handler_data == handler_data)
	match++;
      if (match == target)
	{
	  count++;
	  g_free(handler);
	  list = g_list_delete_link(list, node);
	}
      node = next;
    }

  riscos_display->raw_event_handlers = list;

  return count;
}

void
gdk_riscos_add_message_handler (unsigned message_no,
				gdk_riscos_message_handler handler_func,
				void *data)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);
  message_handler_data *message;

  message = g_new (message_handler_data, 1);
  message->message_no = message_no;
  message->handler_func = handler_func;
  message->data = data;

  riscos_display->message_handlers = g_list_prepend (riscos_display->message_handlers,
						     message);
}

void
gdk_riscos_remove_message_handler (unsigned message_no,
				   gdk_riscos_message_handler handler_func,
				   void *data)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);
  GList *list = riscos_display->message_handlers;
  GList *node = list;

  while (node)
    {
      GList *next = g_list_next(node);
      message_handler_data *handler = node->data;
      if (handler->message_no == message_no &&
	  handler->handler_func == handler_func &&
	  handler->data == data)
	{
	  g_free(handler);
	  list = g_list_delete_link(list, node);
	}
      node = next;
    }

  riscos_display->message_handlers = list;
}

int
gdk_riscos_remove_message_handler_matched (GDKMessageHandlerMatchType mask,
					   unsigned message_no,
					   gdk_riscos_message_handler handler_func,
					   void *data)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);
  GList *list = riscos_display->message_handlers;
  GList *node = list;
  int count = 0;
  int target = __builtin_popcount((unsigned)mask);

  while (node)
    {
      GList *next = g_list_next(node);
      message_handler_data *handler = node->data;
      int match = 0;

      if ((mask & GDK_MESSAGE_HANDLER_MATCH_TYPE) && handler->message_no == message_no)
	match++;
      if ((mask & GDK_MESSAGE_HANDLER_MATCH_FUNC) && handler->handler_func == handler_func)
	match++;
      if ((mask & GDK_MESSAGE_HANDLER_MATCH_DATA) && handler->data == data)
	match++;
      if (match == target)
	{
	  count++;
	  g_free(handler);
	  list = g_list_delete_link(list, node);
	}
      node = next;
    }

  riscos_display->message_handlers = list;

  return count;
}

void
gdk_riscos_remove_message_handler_by_data (void *data)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);
  GList *list = riscos_display->message_handlers;
  GList *node = list;

  while (node)
    {
      GList *next = g_list_next(node);
      message_handler_data *handler = node->data;
      if (handler->data == data)
	{
	  g_free(handler);
	  list = g_list_delete_link(list, node);
	}
      node = next;
    }

  riscos_display->message_handlers = list;
}

static wimp_w INLINE_EVENT_HANDLER
get_event_window (wimp_block *poll_block, wimp_event_no event_type)
{
  wimp_w result = wimp_BACKGROUND;

  switch (event_type)
    {
    case wimp_REDRAW_WINDOW_REQUEST:
      result = poll_block->redraw.w;
      break;
    case wimp_OPEN_WINDOW_REQUEST:
      result = poll_block->open.w;
      break;
    case wimp_CLOSE_WINDOW_REQUEST:
      result = poll_block->close.w;
      break;
    case wimp_POINTER_ENTERING_WINDOW:
    case wimp_POINTER_LEAVING_WINDOW:
      result = poll_block->entering.w;
      break;
    case wimp_MOUSE_CLICK:
      result = poll_block->pointer.w;
      break;
    case wimp_KEY_PRESSED:
      result = poll_block->key.w;
      break;
    case wimp_SCROLL_REQUEST:
      result = poll_block->scroll.w;
      break;
    }

  return result;
}

static gboolean INLINE_EVENT_HANDLER
call_raw_event_handlers (GdkRiscosDisplay *riscos_display,
			 wimp_block *poll_block,
			 wimp_event_no event_type)
{
  GList *list = riscos_display->raw_event_handlers;
  wimp_w event_window = get_event_window (poll_block, event_type);
  while (list)
    {
      raw_event_handler_data *raw_event = list->data;
      GList *next = g_list_next(list);
      if ((raw_event->type == GDK_RISCOS_ANY_WIMP_EVENT || event_type == raw_event->type) &&
	  (raw_event->window_handle == wimp_BACKGROUND || event_window == raw_event->window_handle))
	if (raw_event->handler_func (event_type, poll_block, raw_event->handler_data))
	  return TRUE;

      list = next;
    }

  return FALSE;
}

static gboolean INLINE_EVENT_HANDLER
call_message_handlers (int event_no,
		       GdkRiscosDisplay *riscos_display,
		       wimp_block *poll_block)
{
  GList *list = riscos_display->message_handlers;
  wimp_message *message = &poll_block->message;

  while (list)
    {
      message_handler_data *handler = list->data;
      if (handler->message_no == GDK_RISCOS_ANY_WIMP_MESSAGE || message->action == handler->message_no)
	if (handler->handler_func (event_no, poll_block, handler->data))
	  return TRUE;
      list = g_list_next(list);
    }

  return FALSE;
}

static void
send_event (GdkEvent *event)
{
  GdkDisplay *display = gdk_window_get_display (event->any.window);
  GList *node = _gdk_event_queue_append (display, event);

  _gdk_windowing_got_event (display,
			    node,
			    event,
			    _gdk_display_get_next_serial (display));
}

static os_error *
null_redraw (wimp_draw *redraw)
{
    os_error *err;
    osbool more;

#if DEBUG_REDRAW
    g_warning ("RISCOS: Unrecognised window %p during redraw event.\n", redraw->w);
#endif

    __pthread_disable_ints();

    err = xwimp_redraw_window (redraw, &more);
    if (!err) {
	while (more)
	    err = xwimp_get_rectangle (redraw, &more);
    }

    __pthread_enable_ints();

    return err;
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_redraw_event (GdkRiscosDisplay *riscos_display,
				wimp_block *poll_block)
{
  wimp_draw *redraw = &poll_block->redraw;
  GdkWindow *window;

  window = g_hash_table_lookup (riscos_display->id_ht,
			        redraw->w);
  if (!window)
    {
      null_redraw(redraw);
      return;
    }

  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
  os_coord origin;
#if DEBUG_REDRAW
  os_error *err = NULL;
#endif
  osbool more;

  __pthread_disable_ints();

  xwimp_redraw_window (redraw, &more);

  origin.x = redraw->box.x0 - redraw->xscroll;
  origin.y = redraw->box.y1 - redraw->yscroll - gdk_riscos_window_os_height (window);

  while (more)
    {
#if DEBUG_REDRAW
      err =
#endif
      xosspriteop_put_sprite_scaled (osspriteop_PTR,
				     impl->surface_sprite_area,
				     (osspriteop_id)impl->surface_sprite_pointer,
				     origin.x,
				     origin.y,
				     os_ACTION_OVERWRITE | osspriteop_USE_PALETTE,
				     NULL,//riscos_display->render_scale_ptr, /* NULL means no scaling.  */
				     riscos_display->trans_table);
      xwimp_get_rectangle (redraw, &more);
    }

  __pthread_enable_ints();

#if DEBUG_REDRAW
  if (err)
    g_warning ("RISC OS: Failed to redraw Cairo surface; %s", err->errmess);
#endif
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_open_event (GdkRiscosDisplay *riscos_display,
			      wimp_block *poll_block)
{
  wimp_open *open = &poll_block->open;
  GdkWindow *gwindow;

  gwindow = g_hash_table_lookup (riscos_display->id_ht,
			         open->w);
  if (gwindow == NULL)
    return;

  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (gwindow->impl);

  GdkPoint old_pos;
  gdk_window_get_position (gwindow, &old_pos.x, &old_pos.y);

  GdkPoint new_os_size;
  new_os_size.x = open->visible.x1 - open->visible.x0;
  new_os_size.y = open->visible.y1 - open->visible.y0;
  
  if (new_os_size.x < impl->min_os_size.x || new_os_size.y < impl->min_os_size.y)
    return;

  xwimp_open_window (open);

  GdkPoint old_size;
  old_size.x = gdk_window_get_width (gwindow);
  old_size.y = gdk_window_get_height (gwindow);

  GdkPoint new_size;
  gdk_riscos_screen_point_to_pixel (impl->screen, &new_os_size, &new_size);

  GdkPoint new_pos;
  new_pos.x = open->visible.x0;
  new_pos.y = gdk_riscos_screen_os_height (impl->screen) - open->visible.y1;
  gdk_riscos_screen_point_to_pixel (impl->screen, &new_pos, &new_pos);

  if (old_size.x == new_size.x && old_size.y == new_size.y &&
      old_pos.x == new_pos.x && old_pos.y == new_pos.y)
    return;

  if (new_size.x != old_size.x || new_size.y != old_size.y)
    {
      gwindow->width = new_size.x;
      gwindow->height = new_size.y;
      _gdk_riscos_window_resize_surface (gwindow);
    }

  gwindow->x = new_pos.x;
  gwindow->y = new_pos.y;

  _gdk_window_update_size (gwindow);
  
  GdkEvent *gdk_event;

  gdk_event = gdk_event_new (GDK_CONFIGURE);
  gdk_event->configure.window = g_object_ref (gwindow); 
  gdk_event->configure.send_event = FALSE;
  gdk_event->configure.x = new_pos.x;
  gdk_event->configure.y = new_pos.y;
  gdk_event->configure.width = new_size.x;
  gdk_event->configure.height = new_size.y;

  send_event (gdk_event);
}

/* We don't actually close the window here, we merely report it. It's upto
 * the caller whether the window should be closed or some other action taken.  */
static void INLINE_EVENT_HANDLER
gdk_riscos_handle_close_event (GdkRiscosDisplay *riscos_display,
			       wimp_block *poll_block)
{
  wimp_close *riscos_event = &poll_block->close;
  GdkWindow *window;
  GdkEvent *gdk_event;

  window = g_hash_table_lookup (riscos_display->id_ht,
			        riscos_event->w);
  if (window == NULL)
    return;

  gdk_event = gdk_event_new (GDK_DELETE);
  gdk_event->any.window = g_object_ref (window);
  gdk_event->any.send_event = FALSE;

  send_event (gdk_event);
}

static GdkEvent *
create_focus_event (GdkRiscosDisplay *rdisplay,
		    GdkWindow *window,
		    gboolean   in)
{
  GdkEvent *event;
  GdkSeat *seat = gdk_display_get_default_seat(&rdisplay->parent_instance);

  event = gdk_event_new (GDK_FOCUS_CHANGE);
  event->focus_change.window = g_object_ref (window);
  event->focus_change.in = in;

  gdk_event_set_device (event, gdk_seat_get_keyboard (seat));

  return event;
}

static void
_gdk_riscos_update_focus_window (GdkRiscosDisplay *rdisplay,
				 GdkWindow *window,
				 gboolean got_focus)
{
  GdkEvent *event;

  /* Nothing to do if we already have the input focus.  */
  if (got_focus && window == rdisplay->focus_window)
    return;

  if (!got_focus && window == rdisplay->focus_window)
    {
      /* We just lost the input focus.  */
      event = create_focus_event (rdisplay, rdisplay->focus_window, FALSE);
      send_event (event);
      g_object_unref (rdisplay->focus_window);
      rdisplay->focus_window = NULL;
    }

  if (got_focus)
    {
      /* We gained the input focus...  */
      if (rdisplay->focus_window)
	{
	  /* ...and caused another GDK window to lose it.  */
	  event = create_focus_event (rdisplay, rdisplay->focus_window, FALSE);
	  send_event (event);
	  g_object_unref (rdisplay->focus_window);
	  rdisplay->focus_window = NULL;
	}

      event = create_focus_event (rdisplay, window, TRUE);
      send_event (event);
      rdisplay->focus_window = g_object_ref (window);

      /* Broadcast the fact that we are claiming the selection/caret.  */
      rdisplay->poll_block.message.size = sizeof(wimp_full_message_claim_entity);
      rdisplay->poll_block.message.your_ref = 0;
      rdisplay->poll_block.message.action = message_CLAIM_ENTITY;
      rdisplay->poll_block.message.data.claim_entity.flags = wimp_CLAIM_CARET_OR_SELECTION;
      xwimp_send_message(wimp_USER_MESSAGE, &rdisplay->poll_block.message, wimp_BROADCAST);
    }
}

static void handle_mode_change(GdkRiscosDisplay *rdisplay)
{
#define XEIG_FACTOR 0
#define YEIG_FACTOR 1
#define NCOLOUR 2

  GdkRiscosScreen *screen = GDK_RISCOS_SCREEN (rdisplay->default_screen);
  os_VDU_VAR_LIST(4) vdu = { { os_VDUVAR_XEIG_FACTOR,
				  os_VDUVAR_YEIG_FACTOR,
				  os_VDUVAR_NCOLOUR,
				  os_VDUVAR_END_LIST } };

  xos_read_vdu_variables ((os_vdu_var_list *)&vdu, (int *)&vdu);

  if (vdu.var[XEIG_FACTOR] == screen->x_eigen_factor &&
      vdu.var[YEIG_FACTOR] == screen->y_eigen_factor &&
      (vdu.var[NCOLOUR] == -1 && rdisplay->trans_table == NULL))
    {
      /* If we currently don't have a colourtrans table, and no other
       * attributes have changed, then we have nothing to do.  */
      return;
    }

  _gdk_riscos_screen_update (rdisplay->default_screen);
  _gdk_riscos_display_generate_transtable (rdisplay);
  _gdk_riscos_display_calculate_render_scale (rdisplay);

#undef XEIG_FACTOR
#undef YEIG_FACTOR
#undef NCOLOUR
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_message (int event_no,
			   GdkRiscosDisplay *riscos_display,
			   wimp_block *poll_block)
{
  wimp_message *message = &poll_block->message;

  if (call_message_handlers (event_no, riscos_display, poll_block))
    return;

  if (event_no == wimp_USER_MESSAGE_ACKNOWLEDGE)
    {
      switch (message->action)
	{
	case message_RAM_TRANSMIT:
	  if (riscos_display->clipboard_xfer_progress)
	    {
	      free(riscos_display->clipboard_xfer_progress);
	      riscos_display->clipboard_xfer_progress = NULL;
	    }
	  return;
	}
    }

  switch (message->action)
    {
    case message_MODE_CHANGE:
      handle_mode_change(riscos_display);
      break;

    case message_DATA_LOAD:
      if (message->my_ref == 0) /* 0 = from the filer */
	{
	  GdkWindow *gdk_window = g_hash_table_lookup (riscos_display->id_ht,
						       message->data.data_xfer.w);
	  if (gdk_window)
	    {
#if 0
	      _gdk_riscos_display_set_last_seen_time (rdisplay);

	      GdkEvent *event = gdk_event_new (GDK_KEY_PRESS);
	      event->key.window = g_object_ref (gdk_window);
	      event->key.send_event = FALSE;
	      event->key.time = riscos_display->last_seen_time;
	      event->key.state = GDK_CONTROL_MASK;
	      event->key.keyval = map_keyboard_wimp_to_gdk ('V', GDK_CONTROL_MASK);
	      event->key.length = 0;
	      event->key.string = NULL;
	      event->key.hardware_keycode = event->key.keyval;
	      event->key.group = 0;
	      event->key.is_modifier = FALSE;

	      gdk_event_set_device (event, riscos_display->parent_instance.core_pointer);

	      send_event (event);
#endif
	    }
	}
      break;

    case message_CLAIM_ENTITY: {
      if (message->sender != riscos_display->task_handle)
	{
	  if (message->data.claim_entity.flags & wimp_CLAIM_CARET)
	    {
	      if (riscos_display->focus_window)
		_gdk_riscos_update_focus_window (riscos_display, riscos_display->focus_window, FALSE);
	    }
	  if (message->data.claim_entity.flags & wimp_CLAIM_SELECTION)
	    {
	      gdk_riscos_selection_release (&riscos_display->parent_instance);
	    }
	  if (message->data.claim_entity.flags & wimp_CLAIM_CLIPBOARD)
	    {
	      gdk_riscos_clipboard_free (&riscos_display->parent_instance);
	    }
	}
      break;
    }

    case message_DATA_REQUEST: {
      if (!riscos_display->clipboard.claimed ||
	  (message->data.data_request.flags & wimp_DATA_REQUEST_CLIPBOARD) == 0)
	break;

      message->size = sizeof(wimp_full_message_data_xfer);
      message->your_ref = message->my_ref;
      message->action = message_DATA_SAVE;
      message->data.data_xfer.est_size = riscos_display->clipboard.size;
      message->data.data_xfer.file_type = 0xFFF;
      strcpy(message->data.data_xfer.file_name, "gtk-clipboard");
      xwimp_send_message(wimp_USER_MESSAGE_RECORDED, message, message->sender);

      riscos_display->clipboard_xfer_progress = malloc(sizeof(GdkRiscosXferProgress));
      riscos_display->clipboard_xfer_progress->bytes_remaining = riscos_display->clipboard.size;
      riscos_display->clipboard_xfer_progress->bytes_transfered = 0;

      break;
    }

    case message_RAM_FETCH: {
      if (!riscos_display->clipboard.buffer ||
	  !riscos_display->clipboard_xfer_progress)
	break;

      /* Send as much data as the receiver can handle.  */
      size_t bytes_to_transfer = MIN(message->data.ram_xfer.size, riscos_display->clipboard_xfer_progress->bytes_remaining);
      /* Only send recorded if there is more data to send.  */
      if (bytes_to_transfer > 0)
	{
	  xwimp_transfer_block(riscos_display->task_handle,
			       (guchar *)riscos_display->clipboard.buffer + riscos_display->clipboard_xfer_progress->bytes_transfered,
			       message->sender,
			       message->data.ram_xfer.addr,
			       bytes_to_transfer);
	}

      message->size = sizeof(wimp_full_message_ram_xfer);
      message->your_ref = message->my_ref;
      message->action = message_RAM_TRANSMIT;
      message->data.ram_xfer.size = bytes_to_transfer;
      if (riscos_display->clipboard_xfer_progress->bytes_remaining > 0)
	{
	  /* Only send recorded if there is more data to send.  */
	  xwimp_send_message(wimp_USER_MESSAGE_RECORDED,
			     message,
			     message->sender);

	  riscos_display->clipboard_xfer_progress->bytes_remaining -= bytes_to_transfer;
	  riscos_display->clipboard_xfer_progress->bytes_transfered += bytes_to_transfer;
	}
      else
	{
	  xwimp_send_message(wimp_USER_MESSAGE,
			     message,
			     message->sender);
	  free(riscos_display->clipboard_xfer_progress);
	  riscos_display->clipboard_xfer_progress = NULL;
	}

      break;
    }

    case message_QUIT:
      gdk_riscos_clipboard_release(&riscos_display->parent_instance);
      exit (0);
      break;
    }
}

static guint
translate_mouse_button (wimp_mouse_state mouse_button)
{
  guint state = 0;

  if (mouse_button & (wimp_DOUBLE_SELECT | wimp_SINGLE_SELECT | wimp_DRAG_SELECT))
    state = GDK_BUTTON_PRIMARY;
  else if (mouse_button & wimp_CLICK_MENU)
    state = GDK_BUTTON_MIDDLE;
  else if (mouse_button & (wimp_DOUBLE_ADJUST | wimp_SINGLE_ADJUST | wimp_DRAG_ADJUST))
    state = GDK_BUTTON_SECONDARY;

  return state;
}

/* Read state of mouse buttons.  */
guint
gdk_riscos_translate_buttons (guint mouse_buttons)
{
  guint state = 0;

  if (mouse_buttons & (wimp_DOUBLE_SELECT | wimp_SINGLE_SELECT | wimp_DRAG_SELECT))
    state |= GDK_BUTTON1_MASK;
  if (mouse_buttons & wimp_CLICK_MENU)
    state |= GDK_BUTTON2_MASK;
  if (mouse_buttons & (wimp_DOUBLE_ADJUST | wimp_SINGLE_ADJUST | wimp_DRAG_ADJUST))
    state |= GDK_BUTTON3_MASK;

  return state;
}

/* Read state of keyboard modifier keys.  */
guint
gdk_riscos_read_modifiers (void)
{
  guint state = 0;

  /* Check for shift key. GDK doesn't seem to differentiate between left and right.
   * Use internal key code of 0 which covers both.  */
  if (osbyte1 (osbyte_IN_KEY, 0 ^ 0xff, 0xff))
    state |= GDK_SHIFT_MASK;

  /* Check for control key. Internal key code 1 covers both left and right.  */
  if (osbyte1 (osbyte_IN_KEY, 1 ^ 0xff, 0xff))
    state |= GDK_CONTROL_MASK;

  /* Check for alt key. Internal key code 2 covers both left and right.
   * According to:
   * 
   * https://developer.gnome.org/gdk/stable/gdk-Windows.html#GdkModifierType
   * 
   * the mapping of GDK_MOD1_MASK depends on how the server is configured, but
   * it's usually the Alt key.  */
  if (osbyte1 (osbyte_IN_KEY, 2 ^ 0xff, 0xff))
    state |= GDK_MOD1_MASK;

  /* Check the caps lock key using internal key code of 64.  */
  if (osbyte1 (osbyte_IN_KEY, 64 ^ 0xff, 0xff))
    state |= GDK_LOCK_MASK;

  return state;
}

static void
dump_pointer (wimp_pointer *pointer)
{
  printf ("****\n"
	  "Pointer:\n"
	  " Window: %p\n"
	  " x, y: %d, %d\n"
	  " buttons: %x\n"
	  "****\n",
	  pointer->w,
	  pointer->pos.x,
	  pointer->pos.y,
	  pointer->buttons);
}

static GdkEvent *
create_button_event (GdkRiscosDisplay *rdisplay,
		     GdkWindow *gdk_window,
		     GdkEventType type)
{
  GdkSeat *seat = gdk_display_get_default_seat(&rdisplay->parent_instance);
  GdkEvent *event = gdk_event_new (type);

  event->button.window = g_object_ref (gdk_window);
  event->button.send_event = FALSE;
  event->button.time = rdisplay->last_seen_time;
  event->button.axes = NULL;
  event->button.device = gdk_seat_get_pointer (seat);

  return event;
}

static void
start_resize_window (GdkRiscosDisplay *rdisplay,
		     GdkWindow *window)
{
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (window->impl);
  if (impl->user_can_resize)
    {
      wimp_drag drag;

      drag.w = impl->id;
      drag.type = wimp_DRAG_SYSTEM_SIZE;
      if (xwimp_drag_box(&drag) == NULL)
	{
	  rdisplay->system_drag = TRUE;
	}
    }
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_mouse_click (GdkRiscosDisplay *rdisplay,
			       wimp_block *poll_block)
{
  wimp_pointer *mouse = &poll_block->pointer;

  GdkWindow *gdk_window = g_hash_table_lookup (rdisplay->id_ht, mouse->w);
  if (!gdk_window)
    return;

  GdkRiscosSeat *riscos_seat = GDK_RISCOS_SEAT(gdk_display_get_default_seat(&rdisplay->parent_instance));
  riscos_seat->pointer_grab = gdk_window;

  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (gdk_window->impl);

  GdkPoint mouse_pos, global_pos, local_pos;
  gdk_riscos_screen_point_to_pixel (impl->screen, (GdkPoint *)&mouse->pos, &mouse_pos);
  gdk_riscos_screen_global_point (impl->screen, &mouse_pos, &global_pos);
  gdk_riscos_window_map_from_global (impl->wrapper, &mouse_pos, &local_pos);

  guint modifiers = gdk_riscos_read_modifiers ();
  guint buttons = gdk_riscos_translate_buttons (mouse->buttons);

  gdk_riscos_window_set_caret (gdk_window);

#if 0
  if (buttons & GDK_BUTTON1_MASK && modifiers & GDK_CONTROL_MASK)
    gdk_riscos_window_dump_to_file (gdk_window, "mem:$.surface");
#endif

  GdkEvent *event = create_button_event (rdisplay, gdk_window, GDK_BUTTON_PRESS);
  event->button.x = local_pos.x;
  event->button.y = local_pos.y;
  event->button.state = modifiers | buttons;
  event->button.button = translate_mouse_button (mouse->buttons);
  event->button.x_root = global_pos.x;
  event->button.y_root = global_pos.y;

  send_event (event);

  rdisplay->last_click = *mouse;

  _gdk_riscos_update_focus_window (rdisplay, gdk_window, TRUE);
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_enter_event (GdkRiscosDisplay *rdisplay,
			       wimp_block *poll_block)
{
  wimp_entering *riscos_event = &poll_block->entering;
  wimp_pointer mouse;
  GdkWindow *gdk_window;
  GdkEvent *gdk_event;

  gdk_window = g_hash_table_lookup (rdisplay->id_ht,
				    riscos_event->w);
  if (gdk_window == NULL)
    return;

  GdkSeat *seat = gdk_display_get_default_seat(&rdisplay->parent_instance);

  xwimp_get_pointer_info (&mouse);

  guint modifiers = gdk_riscos_read_modifiers ();
  guint buttons = gdk_riscos_translate_buttons (mouse.buttons);

  _gdk_riscos_display_set_last_seen_time (rdisplay);

  gdk_event = gdk_event_new (GDK_ENTER_NOTIFY);
  gdk_event->crossing.window = g_object_ref (gdk_window);
  gdk_event->crossing.send_event = FALSE;
  gdk_event->crossing.subwindow = g_object_ref (gdk_window);
  gdk_event->crossing.time = rdisplay->last_seen_time;
  gdk_event->crossing.x = 0;
  gdk_event->crossing.y = 0;
  gdk_event->crossing.x_root = 0;
  gdk_event->crossing.y_root = 0;
  gdk_event->crossing.mode = GDK_CROSSING_NORMAL;
  gdk_event->crossing.detail = GDK_NOTIFY_UNKNOWN;
  gdk_event->crossing.focus = FALSE;
  gdk_event->crossing.state = modifiers | buttons;
  
  gdk_event_set_device (gdk_event, gdk_seat_get_pointer (seat));

  send_event (gdk_event);
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_leave_event (GdkRiscosDisplay *rdisplay,
			       wimp_block *poll_block)
{
  wimp_leaving *riscos_event = &poll_block->leaving;
  wimp_pointer mouse;
  GdkWindow *gdk_window;
  GdkEvent *gdk_event;

  gdk_window = g_hash_table_lookup (rdisplay->id_ht,
				    riscos_event->w);
  if (gdk_window == NULL)
    return;

  GdkSeat *seat = gdk_display_get_default_seat(&rdisplay->parent_instance);

  xwimp_get_pointer_info (&mouse);

  guint modifiers = gdk_riscos_read_modifiers ();
  guint buttons = gdk_riscos_translate_buttons (mouse.buttons);

  _gdk_riscos_display_set_last_seen_time (rdisplay);

  gdk_event = gdk_event_new (GDK_LEAVE_NOTIFY);
  gdk_event->crossing.window = g_object_ref (gdk_window);
  gdk_event->crossing.send_event = FALSE;
  gdk_event->crossing.subwindow = g_object_ref (gdk_window);
  gdk_event->crossing.time = rdisplay->last_seen_time;
  gdk_event->crossing.x = 0;
  gdk_event->crossing.y = 0;
  gdk_event->crossing.x_root = 0;
  gdk_event->crossing.y_root = 0;
  gdk_event->crossing.mode = GDK_CROSSING_NORMAL;
  gdk_event->crossing.detail = GDK_NOTIFY_UNKNOWN;
  gdk_event->crossing.focus = FALSE;
  gdk_event->crossing.state = modifiers | buttons;

  gdk_event_set_device (gdk_event, gdk_seat_get_pointer (seat));

  send_event (gdk_event);
}

/*
 * Return -1 if there is no mapping.  */
static int keyboard_map_wimp_to_internal (GdkRiscosDisplay *riscos_display, int wimp_key)
{
  int result = GPOINTER_TO_INT(g_hash_table_lookup (riscos_display->inkey_hashtable,
						    GINT_TO_POINTER(wimp_key)));

  return result ? result : -1;
}

static guint
map_keyboard_wimp_to_gdk (GdkRiscosDisplay *riscos_display, int key, guint modifiers)
{
  guint result;

  if (key <= 26 && (modifiers & GDK_CONTROL_MASK))
    return key + 'a' - 1;

  result = GPOINTER_TO_INT(g_hash_table_lookup (riscos_display->gdkkey_hashtable,
						GINT_TO_POINTER(key)));
  if (!result)
    result = (guint)key;

  return result;
}

#if 0
static void dump_keys_down(GdkRiscosDisplay *rdisplay)
{
  GSList *iterator = rdisplay->key_down_list;

  log_printf("  key down list:");
  while (iterator)
    {
      GSList *next = iterator->next;
      int code = GPOINTER_TO_INT (iterator->data);
      int inkey_code = keyboard_map_wimp_to_internal (code);

      log_printf("    inkey (%d), code (%d, '%c')",inkey_code,code,code);
      iterator = next;
    }
}
#endif

/* Clean out from the list any keys that were released while not over a
   GDK window.
   FIXME: Should we be checking for a grab window and sending an event to it?  */
static void
check_key_down_no_window (GdkRiscosDisplay *rdisplay)
{
  GSList *iterator = rdisplay->key_down_list;

  while (iterator)
    {
      GSList *next = iterator->next;
      int code = GPOINTER_TO_INT (iterator->data);
      int inkey_code = keyboard_map_wimp_to_internal (rdisplay, code);

      if (inkey_code >= 0)
	{
	  if (osbyte1 (osbyte_IN_KEY, inkey_code ^ 0xff, 0xff) == 0)
	    rdisplay->key_down_list = g_slist_delete_link (rdisplay->key_down_list, iterator);
	}
      else if (inkey_code == -1)
	{
	  /* FIXME: There shouldn't be any unidentifed keys, for now just remove them from
	   * the list.  */
	  rdisplay->key_down_list = g_slist_delete_link (rdisplay->key_down_list, iterator);
	}
      iterator = next;
    }
}

static void
check_key_down (GdkRiscosDisplay *rdisplay,
		GdkWindow *gdk_window,
		guint state)
{
  GSList *iterator = rdisplay->key_down_list;
  GdkSeat *seat = gdk_display_get_default_seat(&rdisplay->parent_instance);

  while (iterator)
    {
      GSList *next = iterator->next;
      int code = GPOINTER_TO_INT (iterator->data);
      int inkey_code = keyboard_map_wimp_to_internal (rdisplay, code);

      if (inkey_code >= 0 && osbyte1 (osbyte_IN_KEY, inkey_code ^ 0xff, 0xff) == 0)
	  {
	    _gdk_riscos_display_set_last_seen_time (rdisplay);

	    GdkEvent *event = gdk_event_new (GDK_KEY_RELEASE);
	    event->key.window = g_object_ref (gdk_window);
	    event->key.send_event = FALSE;
	    event->key.time = rdisplay->last_seen_time;
	    event->key.state = state;
	    event->key.keyval = map_keyboard_wimp_to_gdk (rdisplay, code, state);
	    event->key.length = 0;
	    event->key.string = NULL;
	    event->key.hardware_keycode = event->key.keyval;
	    event->key.group = 0;
	    event->key.is_modifier = FALSE;

	    gdk_event_set_device (event, gdk_seat_get_pointer (seat));

	    send_event (event);
#if 0
	    {
	      char buffer[150];
	      
	      sprintf (buffer, "KEY_RELEASE inkey (%d), wimp (%d) state (%X) '%c'",inkey_code, code,event->key.state, code);
	      report_text0 (buffer);
	    }
#endif
	    rdisplay->key_down_list = g_slist_delete_link (rdisplay->key_down_list, iterator);
	  }
	else if (inkey_code == -1)
	  {
	    /* FIXME: There shouldn't be any unidentifed keys, for now just remove them from
	     * the list.  */
	    rdisplay->key_down_list = g_slist_delete_link (rdisplay->key_down_list, iterator);
	  }

      iterator = next;
    }
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_null_event (GdkRiscosDisplay *rdisplay)
{
  GdkEvent *event;
  wimp_pointer mouse;

  /* If the task has no windows, then nothing to do here.  */
  if (rdisplay->toplevels == NULL)
    return;

  xwimp_get_pointer_info (&mouse);
  
  GdkWindow *mouse_window = g_hash_table_lookup (rdisplay->id_ht, mouse.w);
  if (!mouse_window)
    {
      check_key_down_no_window (rdisplay);
      return;
    }

  GdkRiscosSeat *riscos_seat = GDK_RISCOS_SEAT(gdk_display_get_default_seat(&rdisplay->parent_instance));
  GdkWindow *event_window = riscos_seat->pointer_grab ? : mouse_window;
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (event_window->impl);

  GdkPoint mouse_pos, global_pos, local_pos;
  gdk_riscos_screen_point_to_pixel (impl->screen, (GdkPoint *)&mouse.pos, &mouse_pos);
  gdk_riscos_screen_global_point (impl->screen, &mouse_pos, &global_pos);
  gdk_riscos_window_map_from_global (impl->wrapper, &mouse_pos, &local_pos);

  guint modifiers = gdk_riscos_read_modifiers ();
  guint buttons = gdk_riscos_translate_buttons (mouse.buttons);
  guint old_buttons = gdk_riscos_translate_buttons (rdisplay->last_click.buttons);

  check_key_down (rdisplay,
		  mouse_window,
		  modifiers | buttons);

  if (rdisplay->system_drag)
    {
      if (old_buttons != 0 && buttons == 0)
	rdisplay->system_drag = FALSE;
      return;
    }
  else
    {
      if (old_buttons != 0 && buttons == 0)
	{
	  if (riscos_seat->pointer_grab)
	    riscos_seat->pointer_grab = NULL;

	  _gdk_riscos_display_set_last_seen_time (rdisplay);

	  event = create_button_event (rdisplay,
				       event_window,
				       GDK_BUTTON_RELEASE);

	  event->button.x = local_pos.x;
	  event->button.y = local_pos.y;
	  event->button.state = modifiers;
	  event->button.button = translate_mouse_button (rdisplay->last_click.buttons);
	  event->button.x_root = global_pos.x;
	  event->button.y_root = global_pos.y;

	  send_event (event);

	  rdisplay->last_click.buttons = 0;

	  return;
	}

      /* Check for the user starting a window resize drag.  */
      guint current_button = translate_mouse_button (mouse.buttons);
      
      if (current_button == GDK_BUTTON_PRIMARY && modifiers & GDK_SHIFT_MASK)
	{
	  start_resize_window (rdisplay, mouse_window);
	  return;
	}
    }

  /* If the pointer hasn't moved, then don't send a motion event.  */
  if (mouse_pos.x == rdisplay->last_mouse_move.x &&
      mouse_pos.y == rdisplay->last_mouse_move.y)
    return;

  rdisplay->last_mouse_move.x = mouse_pos.x;
  rdisplay->last_mouse_move.y = mouse_pos.y;

  /* Check if this window has requested to be notified of motion events.
     TODO: Reduce the number of events for MOTION_HINT and check for button
     presses for BUTTON_MOTION masks.  */
  GdkEventMask event_mask = gdk_window_get_events (mouse_window) | impl->event_mask;
  if ((event_mask & (GDK_POINTER_MOTION_MASK |
		    GDK_POINTER_MOTION_HINT_MASK |
		    GDK_BUTTON_MOTION_MASK |
		    GDK_BUTTON1_MOTION_MASK |
		    GDK_BUTTON2_MOTION_MASK |
		    GDK_BUTTON3_MOTION_MASK)) == 0)
    return;

  /* If the local_pos.y is -ve, then don't send a motion event; this can happen when
     dragging the window around by the title bar.  */
  if (local_pos.y < 0)
    return;

  _gdk_riscos_display_set_last_seen_time (rdisplay);

  event = gdk_event_new (GDK_MOTION_NOTIFY);
  event->motion.window = g_object_ref (event_window/*mouse_window*/);
  event->motion.send_event = FALSE;
  event->motion.time = rdisplay->last_seen_time;
  event->motion.x = local_pos.x;
  event->motion.y = local_pos.y;
  event->motion.axes = NULL;
  event->motion.state = modifiers | buttons;
  event->motion.is_hint = FALSE;
  event->motion.x_root = global_pos.x;
  event->motion.y_root = global_pos.y;
  gdk_event_set_device (event, gdk_seat_get_pointer (&riscos_seat->parent_instance));
  gdk_event_set_seat (event, &riscos_seat->parent_instance);
#if 0
{
  char buffer[150];

  sprintf (buffer, "MOTION_NOTIFY : local(%f,%f) : global(%f,%f)",event->motion.x,event->motion.y,event->motion.x_root,event->motion.y_root);
  report_text0 (buffer);
}
#endif
  send_event (event);
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_key_press (GdkRiscosDisplay *rdisplay,
			     wimp_block *poll_block)
{
  wimp_key *key = &poll_block->key;

  switch (key->c)
  {
  case 3: /* CTRL-C */
/*    if (event->key.state & GDK_CONTROL_MASK)*/
      {
	gdk_riscos_clipboard_claim(&rdisplay->parent_instance,
				   rdisplay->selection.buffer,
				   rdisplay->selection.size);
      }
    return;

  case KEYCODE_F12:
  case KEYCODE_SHIFT_F12:
  case KEYCODE_CTRL_F12:
  case KEYCODE_CTRL_SHIFT_F12:
    xwimp_process_key(key->c);
    return;
  }

  GdkWindow *gdk_window = g_hash_table_lookup (rdisplay->id_ht, key->w);
  if (!gdk_window)
    return;

  GdkSeat *seat = gdk_display_get_default_seat (&rdisplay->parent_instance);

  wimp_pointer mouse;
  xwimp_get_pointer_info (&mouse);

  /* Make sure top bit set characters entered via the ALT key can get through.  */
  guint modifiers = ((key->c & 0x80) == 0) ?
			gdk_riscos_read_modifiers () :
			0;

  guint buttons = gdk_riscos_translate_buttons (mouse.buttons);

  _gdk_riscos_display_set_last_seen_time (rdisplay);

  GdkEvent *event = gdk_event_new (GDK_KEY_PRESS);
  event->key.window = g_object_ref (gdk_window);
  event->key.send_event = FALSE;
  event->key.time = rdisplay->last_seen_time;
  event->key.state = modifiers | buttons;
  event->key.keyval = map_keyboard_wimp_to_gdk (rdisplay, key->c, modifiers);
  event->key.length = 0;
  event->key.string = NULL;
  event->key.hardware_keycode = event->key.keyval;
  event->key.group = 0;
  event->key.is_modifier = FALSE;

  gdk_event_set_device (event, gdk_seat_get_keyboard (seat));

  send_event (event);
#if 0
    {
      char buffer[150];

      sprintf (buffer, "KEY_PRESS wimp (%d) state (%X) '%c'",key->c,event->key.state, key->c);
      report_text0 (buffer);
    }
#endif
  rdisplay->key_down_list = g_slist_prepend (rdisplay->key_down_list,
					     GUINT_TO_POINTER (key->c));
}

static void INLINE_EVENT_HANDLER
gdk_riscos_handle_scroll_request(GdkRiscosDisplay *rdisplay,
				 wimp_block *poll_block)
{
  wimp_scroll *scroll = &poll_block->scroll;

  GdkWindow *gdk_window = g_hash_table_lookup (rdisplay->id_ht, scroll->w);
  if (!gdk_window)
    return;

  GdkSeat *seat = gdk_display_get_default_seat(&rdisplay->parent_instance);
  GdkWindowImplRiscos *impl = GDK_WINDOW_IMPL_RISCOS (gdk_window->impl);

  wimp_pointer mouse;
  xwimp_get_pointer_info (&mouse);

  GdkPoint mouse_pos, global_pos, local_pos;
  gdk_riscos_screen_point_to_pixel (impl->screen, (GdkPoint *)&mouse.pos, &mouse_pos);
  gdk_riscos_screen_global_point (impl->screen, &mouse_pos, &global_pos);
  gdk_riscos_window_map_from_global (impl->wrapper, &mouse_pos, &local_pos);

  guint modifiers = gdk_riscos_read_modifiers ();
  guint buttons = gdk_riscos_translate_buttons (mouse.buttons);

  _gdk_riscos_display_set_last_seen_time (rdisplay);

  GdkEvent *event = gdk_event_new (GDK_SCROLL);
  event->scroll.window = g_object_ref (gdk_window);
  event->scroll.send_event = FALSE;
  event->scroll.time = rdisplay->last_seen_time;
  event->scroll.x = local_pos.x;
  event->scroll.y = local_pos.y;
  event->scroll.state = modifiers | buttons;
  event->scroll.delta_x = event->scroll.delta_y = 0;

  gboolean extended_scroll = (scroll->ymin && (scroll->ymin & 3) == 0) ||
			     (scroll->xmin && (scroll->xmin & 3) == 0);
  if (extended_scroll)
    {
      event->scroll.direction = GDK_SCROLL_SMOOTH;
      event->scroll.delta_x = (scroll->xmin / 4) * SCROLL_X_DELTA;
      event->scroll.delta_y = -(scroll->ymin / 4) * SCROLL_Y_DELTA;
    }
  else if (scroll->xmin)
    event->scroll.direction = (scroll->xmin < 0) ? GDK_SCROLL_LEFT : GDK_SCROLL_RIGHT;
  else if (scroll->ymin)
    event->scroll.direction = (scroll->ymin < 0) ? GDK_SCROLL_DOWN : GDK_SCROLL_UP;

  event->scroll.device = gdk_seat_get_pointer (seat);
  event->scroll.x_root = global_pos.x;
  event->scroll.y_root = global_pos.y;

  send_event (event);
}

static gboolean
gdk_event_prepare (GSource *source,
		   gint    *timeout)
{
  *timeout = 0;

  return TRUE;
}

static gboolean
gdk_event_check (GSource *source)
{
  return TRUE;
}

static gboolean
gdk_event_dispatch (GSource     *source,
		    GSourceFunc  callback,
		    gpointer     user_data)
{
  GdkDisplay *display = ((GdkEventSource*) source)->display;
  GdkEvent *event;

  event = gdk_display_get_event (display);

  if (event)
    {
      _gdk_event_emit (event);

      gdk_event_free (event);
    }

  return TRUE;
}

static GSourceFuncs event_funcs = {
  gdk_event_prepare,
  gdk_event_check,
  gdk_event_dispatch,
  NULL
};

static void
_gdk_riscos_event_loop_init (GdkDisplay *display)
{
  GSource *source;
  GdkEventSource *event_source;

  /* Hook into the GLib main loop */
  source = g_source_new (&event_funcs, sizeof (GdkEventSource));
  g_source_set_name (source, "GDK RISC OS WIMP Event source"); 
  event_source = (GdkEventSource *) source;
  event_source->display = display;

  /* Using a priority of GDK_PRIORITY_EVENTS prevents the widget expose
   * signals from getting through, so use a lower priority.  */
  g_source_set_priority (source, GDK_PRIORITY_REDRAW + 5);
  g_source_set_can_recurse (source, TRUE);
  g_source_attach (source, NULL);
}

static wimp_event_no
poll_wimp_event (GdkRiscosDisplay *rdisplay,
		 wimp_block *poll_block)
{
  wimp_event_no event_no;

  xwimp_poll (0, poll_block, NULL, &event_no);

  if (!call_raw_event_handlers (rdisplay, poll_block, event_no))
    {
      switch (event_no)
      {
      case wimp_NULL_REASON_CODE:
	gdk_riscos_handle_null_event (rdisplay);
	break;
      case wimp_REDRAW_WINDOW_REQUEST:
	gdk_riscos_handle_redraw_event (rdisplay, poll_block);
	break;
      case wimp_OPEN_WINDOW_REQUEST:
	gdk_riscos_handle_open_event (rdisplay, poll_block);
	break;
      case wimp_CLOSE_WINDOW_REQUEST:
	gdk_riscos_handle_close_event (rdisplay, poll_block);
	break;
      case wimp_POINTER_ENTERING_WINDOW:
	gdk_riscos_handle_enter_event (rdisplay, poll_block);
	break;
      case wimp_POINTER_LEAVING_WINDOW:
	gdk_riscos_handle_leave_event (rdisplay, poll_block);
	break;
      case wimp_MOUSE_CLICK:
	gdk_riscos_handle_mouse_click (rdisplay, poll_block);
	break;
      case wimp_KEY_PRESSED:
	gdk_riscos_handle_key_press (rdisplay, poll_block);
	break;
      case wimp_SCROLL_REQUEST:
	gdk_riscos_handle_scroll_request (rdisplay, poll_block);
	break;
#if 0
      /* NOTE: The RISC OS clipboard model states that we should ignore
       * the wimp_LOSE_CARET/wimp_GAIN_CARET events and use message_CLAIM_ENTITY
       * instead.  */
      case wimp_LOSE_CARET:
	gdk_riscos_handle_lose_focus_event (rdisplay, poll_block);
	break;
      case wimp_GAIN_CARET:
	gdk_riscos_handle_gain_focus_event (rdisplay, poll_block);
	break;
#endif
      case wimp_USER_MESSAGE:
      case wimp_USER_MESSAGE_RECORDED:
      case wimp_USER_MESSAGE_ACKNOWLEDGE:
	gdk_riscos_handle_message (event_no, rdisplay, poll_block);
	break;
      }
    }

  return event_no;
}

void
_gdk_riscos_display_queue_events (GdkDisplay *display)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  poll_wimp_event (riscos_display, &riscos_display->poll_block);

  pthread_yield();
}

wimp_event_no
gdk_riscos_event_poll (wimp_block *poll_block)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY(_gdk_display);

  return poll_wimp_event (riscos_display, poll_block);
}

void
_gdk_riscos_events_init (GdkDisplay *display)
{
  GdkRiscosDisplay *riscos_display = GDK_RISCOS_DISPLAY (display);

  for (int i = 0; i < sizeof (function_keymap) / sizeof (struct keymapping); i++)
    g_hash_table_insert (riscos_display->gdkkey_hashtable,
			 GINT_TO_POINTER (function_keymap[i].riscos_key_code),
			 GINT_TO_POINTER (function_keymap[i].gdk_key_code));

  for (int i = 0; i < sizeof (internal_keymap) / sizeof (struct internal_mapping); i++)
    g_hash_table_insert (riscos_display->inkey_hashtable,
			 GINT_TO_POINTER (internal_keymap[i].wimp_code),
			 GINT_TO_POINTER (internal_keymap[i].internal_code));

  _gdk_riscos_event_loop_init (display);
}
