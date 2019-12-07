#include <stdlib.h>
#include "gdkriscos.h"
#include "gdkdisplay-riscos.h"
#include "gdkprivate-riscos.h"

#include "oslib/clipboard.h"

typedef struct
{
  GdkRiscosDisplay *riscos_display;
  gpointer data;
} WaitResults;

static gboolean
clipboard_data_type_is_cb(wimp_event_no event, wimp_block *block, void *data)
{
  clipboard_full_message_data_type_is *message = (clipboard_full_message_data_type_is *)block;
  WaitResults *results = (WaitResults *)data;
  results->data = (void *)message->file_type;

  return TRUE;
}

gboolean
gdk_riscos_clipboard_is_text_available(GdkDisplay *display)
{
  WaitResults results;
  int types[] = { 0xFFF, -1 };

  results.riscos_display = GDK_RISCOS_DISPLAY(display);
  results.data = NULL;

  GdkWindow *focus = results.riscos_display->focus_window;
  if (!focus)
    return FALSE;

  gdk_riscos_add_message_handler(message_DATA_TYPE_IS, clipboard_data_type_is_cb, &results);

  os_error *err = xclipboard_get_data_type(0,
					   gdk_riscos_window_get_handle(focus),
					   /*icon=*/-1,
					   /*x=*/0,
					   /*y=*/0,
					   types);
  if (err)
    {
      g_warning ("gdk_riscos_clipboard_is_text_available: RISC OS error: %s\n", err->errmess);
      return FALSE;
    }

  /* We should get a reply within 10 WIMP polls.  */
  for (int i = 10; i; --i)
    {
      g_main_context_iteration(NULL, FALSE);
      if (results.data)
	break;
    }

  gdk_riscos_remove_message_handler(message_DATA_TYPE_IS, clipboard_data_type_is_cb, &results);

  return (unsigned)results.data == 0xFFF;
}

static gboolean
clipboard_paste_cb(wimp_event_no event, wimp_block *block, void *data)
{
  clipboard_full_message_paste *message = (clipboard_full_message_paste *)block;
  WaitResults *results = (WaitResults *)data;

  results->data = malloc(message->data_length + 1);
  xwimp_transfer_block(message->sender,
		       message->data,
		       results->riscos_display->task_handle,
		       results->data,
		       message->data_length);
  *((char *)results->data + message->data_length) = '\0';

  return TRUE;
}

gchar *
gdk_riscos_clipboard_request_text(GdkDisplay *display)
{
  WaitResults results;
  int types[] = { 0xFFF, -1 };

  results.riscos_display = GDK_RISCOS_DISPLAY(display);
  results.data = NULL;

  GdkWindow *focus = results.riscos_display->focus_window;
  if (!focus)
    return FALSE;

  gdk_riscos_add_message_handler(message_PASTE, clipboard_paste_cb, &results);

  xclipboard_get(0,
		 gdk_riscos_window_get_handle(focus),
		 /*icon=*/-1,
		 /*x=*/0,
		 /*y=*/0,
		 types);

  /* We should get a reply within 10 WIMP polls.  */
  for (int i = 10; i; --i)
    {
      g_main_context_iteration(NULL, FALSE);
      if (results.data)
	break;
    }

  gdk_riscos_remove_message_handler(message_PASTE, clipboard_paste_cb, &results);

  return results.data;
}
