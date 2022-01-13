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
 * Written by Lee Noar.
 */

#include "config.h"

#include <stdlib.h>
#include "gdkriscos.h"
#include "gdkdisplay-riscos.h"
#include "gdkprivate-riscos.h"
#include <oslib/osfile.h>
#include <stdint.h>

#define STATUS_RUNNING 0
#define STATUS_FINISHED 1
#define STATUS_ERROR 2

#define BUFFER_SIZE 1024

typedef struct
{
  wimp_w w;
  GdkRiscosDisplay *riscos_display;
  GdkRiscosXferProgress progress;
  int ram_fetch_count;
  uint8_t *result;
  uint8_t *buffer;

  /* 0 - still processing
   * 1 - Finished without error
   * 2 - Finished with error
   */
  int status;

} Transfer;

static const char scrapfile[] = "<Wimp$Scrap>";

static bool transfer_init(Transfer *transfer,
			  GdkRiscosDisplay *display)
{
  GdkWindow *focus = display->focus_window;
  if (!focus)
    return false;

  transfer->w = gdk_riscos_window_get_handle(focus);
  transfer->riscos_display = display;
  transfer->progress.bytes_remaining = 0;
  transfer->progress.bytes_transfered = 0;
  transfer->result = NULL;
  transfer->buffer = NULL;
  transfer->ram_fetch_count = 0;
  transfer->status = STATUS_RUNNING;

  return true;
}

static void
transfer_fini(Transfer *transfer)
{
  g_free(transfer->buffer);
}

static gboolean
clipboard_data_type_is_cb(wimp_event_no event, wimp_block *block, void *data)
{
  wimp_full_message_data_xfer *message = (wimp_full_message_data_xfer *)block;
  Transfer *transfer = (Transfer *)data;
  transfer->result = (void *)message->file_type;
  return TRUE;
}

gboolean
gdk_riscos_clipboard_is_text_available(GdkDisplay *display)
{
  GdkRiscosDisplay *rdisplay = GDK_RISCOS_DISPLAY(display);

  if (rdisplay->clipboard.claimed)
    return TRUE;

  Transfer transfer;
  if (!transfer_init (&transfer, rdisplay))
    return FALSE;

  wimp_full_message_data_request *message = (wimp_full_message_data_request *)&rdisplay->poll_block;
  message->size = sizeof(wimp_message_data_request);
  message->your_ref = 0;
  message->action = message_DATA_REQUEST;
  message->w = transfer.w;
  message->i = -1;
  message->pos.x = 0;
  message->pos.y = 0;
  message->flags = 4;
  message->file_types[0] = 0xFFF;
  message->file_types[1] = -1;
  xwimp_send_message(wimp_USER_MESSAGE, &rdisplay->poll_block.message, wimp_BROADCAST);

  gdk_riscos_add_message_handler (message_DATA_SAVE, clipboard_data_type_is_cb, &transfer);

  /* We should get a reply within 10 WIMP polls.  */
  for (int i = 10; i; --i)
    {
      g_main_context_iteration(NULL, FALSE);
      if (transfer.result)
	break;
    }

  gdk_riscos_remove_message_handler (message_DATA_SAVE, clipboard_data_type_is_cb, &transfer);

  transfer_fini (&transfer);

  return (unsigned)transfer.result == 0xFFF;
}

static gboolean
clipboard_datasave_cb(wimp_event_no event, wimp_block *block, void *data)
{
  wimp_full_message_ram_xfer *message = (wimp_full_message_ram_xfer *)block;
  Transfer *transfer = (Transfer *)data;

  int est_size = block->message.data.data_xfer.est_size;
  transfer->result = g_try_malloc(est_size + 1);
  if (!transfer->result)
    {
      transfer->status = STATUS_ERROR;
      return TRUE;
    }

  transfer->buffer = g_try_malloc(BUFFER_SIZE);
  if (!transfer->buffer)
    {
      transfer->status = STATUS_ERROR;
      g_free(transfer->result);
      return TRUE;
    }

  transfer->progress.bytes_remaining = est_size;
  transfer->progress.bytes_transfered = 0;

  message->size = sizeof(wimp_full_message_ram_xfer);
  message->your_ref = message->my_ref;
  message->action = message_RAM_FETCH;
  message->addr = transfer->buffer;
  message->xfer_size = BUFFER_SIZE;
  xwimp_send_message(wimp_USER_MESSAGE_RECORDED, &block->message, message->sender);

  transfer->ram_fetch_count++;

  return TRUE;
}

static gboolean
clipboard_ram_transmit_cb(wimp_event_no event, wimp_block *block, void *data)
{
  wimp_full_message_ram_xfer *message = (wimp_full_message_ram_xfer *)block;
  Transfer *transfer = (Transfer *)data;

  size_t bytes_written = message->xfer_size;

  if (bytes_written)
    memcpy(transfer->result + transfer->progress.bytes_transfered,
	   transfer->buffer,
	   bytes_written);

  transfer->progress.bytes_transfered += bytes_written;

  if (bytes_written < BUFFER_SIZE)
    {
      *((int8_t *)transfer->result + transfer->progress.bytes_transfered) = 0;
      transfer->status = STATUS_FINISHED;
      return TRUE;
    }

  transfer->progress.bytes_remaining -= bytes_written;

  message->size = sizeof(wimp_full_message_ram_xfer);
  message->your_ref = message->my_ref;
  message->action = message_RAM_FETCH;
  message->addr = transfer->buffer;
  message->xfer_size = BUFFER_SIZE;

  xwimp_send_message(wimp_USER_MESSAGE_RECORDED, &block->message, message->sender);

  transfer->ram_fetch_count++;

  return TRUE;
}

static gboolean
clipboard_ram_fetch_cb(wimp_event_no event, wimp_block *block, void *data)
{
  if (event == wimp_USER_MESSAGE_ACKNOWLEDGE)
    {
      /* The RAM_FETCH message bounced.  */
      Transfer *transfer = (Transfer *)data;

      if (transfer->ram_fetch_count == 1)
	{
	  /* If it's the first RAM_FETCH, then revert to file transfer.  */
	  wimp_full_message_data_xfer *message = (wimp_full_message_data_xfer *)block;
	  message->your_ref = message->my_ref;
	  message->size = ((offsetof(wimp_full_message_data_xfer, file_name) + strlen(scrapfile) + 1) + 3) & ~3;
	  message->action = message_DATA_SAVE_ACK;
	  message->w = transfer->w;
	  message->i = -1;
	  message->pos.x = 0;
	  message->pos.y = 0;
	  message->est_size = -1;
	  message->file_type = 0xFFF;
	  strcpy(message->file_name, scrapfile);
	  if (xwimp_send_message(wimp_USER_MESSAGE, &block->message, message->sender))
	    {
	      transfer->status = STATUS_ERROR;
	      return TRUE;
	    }
	}
      else
	{
	  /* If it wasn't the first RAM_FETCH that bounced, then something went wrong,
	   * signal an error.  */
	  transfer->status = STATUS_ERROR;
	  return TRUE;
	}
    }

  return TRUE;
}

static gboolean
clipboard_data_load_cb(wimp_event_no event, wimp_block *block, void *data)
{
  wimp_full_message_data_xfer *message = (wimp_full_message_data_xfer *)block;
  Transfer *transfer = (Transfer *)data;
  int size;

  if (xosfile_read_stamped(message->file_name,
			   NULL, NULL, NULL,
			   &size,
			   NULL, NULL))
    return TRUE;

  transfer->result = g_try_malloc(size + 1);
  if (!transfer->result)
    {
      transfer->status = STATUS_ERROR;
      return TRUE;
    }

  if (xosfile_load_stamped_no_path(message->file_name, transfer->result,
				   NULL, NULL, NULL, NULL, NULL))
    {
      g_free(transfer->result);
      transfer->result = NULL;
      transfer->status = STATUS_ERROR;
      return TRUE;
    }

  /* Make sure we terminate the text we load from the scrap file.  */
  *((char *)transfer->result + size) = '\0';

  xosfile_delete(scrapfile, NULL, NULL, NULL, NULL, NULL);

  message->your_ref = message->my_ref;
  message->action = message_DATA_LOAD_ACK;
  xwimp_send_message(wimp_USER_MESSAGE, &block->message, message->sender);

  transfer->status = STATUS_FINISHED;

  return TRUE;
}

gchar *
gdk_riscos_clipboard_request_text(GdkDisplay *display)
{
  GdkRiscosDisplay *rdisplay = GDK_RISCOS_DISPLAY(display);

  if (rdisplay->clipboard.claimed)
  {
    char *result = g_try_malloc(rdisplay->clipboard.size + 1);
    if (result)
      {
	memcpy(result, rdisplay->clipboard.buffer, rdisplay->clipboard.size);
	*((int8_t *)result + rdisplay->clipboard.size) = 0;
      }
    return result;
  }

  Transfer transfer;
  if (!transfer_init(&transfer, rdisplay))
    return NULL;

  wimp_full_message_data_request *message = (wimp_full_message_data_request *)&rdisplay->poll_block;
  message->size = sizeof(wimp_message_data_request);
  message->your_ref = 0;
  message->action = message_DATA_REQUEST;
  message->w = transfer.w;
  message->i = -1;
  message->pos.x = 0;
  message->pos.y = 0;
  message->flags = 4;
  message->file_types[0] = 0xFFF;
  message->file_types[1] = -1;
  xwimp_send_message(wimp_USER_MESSAGE, &rdisplay->poll_block.message, wimp_BROADCAST);

  gdk_riscos_add_message_handler(message_DATA_SAVE, clipboard_datasave_cb, &transfer);
  gdk_riscos_add_message_handler(message_RAM_TRANSMIT, clipboard_ram_transmit_cb, &transfer);
  gdk_riscos_add_message_handler(message_RAM_FETCH, clipboard_ram_fetch_cb, &transfer);
  gdk_riscos_add_message_handler(message_DATA_LOAD, clipboard_data_load_cb, &transfer);

  while (transfer.status == STATUS_RUNNING)
    g_main_context_iteration(NULL, FALSE);

  gdk_riscos_remove_message_handler(message_DATA_LOAD, clipboard_data_load_cb, &transfer);
  gdk_riscos_remove_message_handler(message_RAM_FETCH, clipboard_ram_fetch_cb, &transfer);
  gdk_riscos_remove_message_handler(message_RAM_TRANSMIT, clipboard_ram_transmit_cb, &transfer);
  gdk_riscos_remove_message_handler(message_DATA_SAVE, clipboard_datasave_cb, &transfer);

  transfer_fini(&transfer);

  return transfer.status == STATUS_FINISHED ? (gchar *)transfer.result : NULL;
}

void
gdk_riscos_clipboard_claim(GdkDisplay *display, const void *data, size_t size)
{
  GdkRiscosDisplay *rdisplay = GDK_RISCOS_DISPLAY(display);

  if (!data || !size)
    {
      data = rdisplay->selection.buffer;
      size = rdisplay->selection.size;
    }

  if (!data || !size)
    return;

  if (rdisplay->clipboard.buffer)
    free(rdisplay->clipboard.buffer);

  rdisplay->clipboard.buffer = malloc(size + 1);
  if (!rdisplay->clipboard.buffer)
      return;
  memcpy(rdisplay->clipboard.buffer, data, size);

  rdisplay->clipboard.size = size;
  rdisplay->clipboard.max_size = rdisplay->clipboard.size;

  if (!rdisplay->clipboard.claimed)
    {
      wimp_message message;
      message.size = sizeof(wimp_full_message_claim_entity);
      message.your_ref = 0;
      message.action = message_CLAIM_ENTITY;
      message.data.claim_entity.flags = wimp_CLAIM_CLIPBOARD;
      xwimp_send_message(wimp_USER_MESSAGE, &message, wimp_BROADCAST);
      rdisplay->clipboard.claimed = TRUE;
    }
}

void
gdk_riscos_clipboard_release(GdkDisplay *display)
{
  GdkRiscosDisplay *rdisplay = GDK_RISCOS_DISPLAY(display);

  if (rdisplay->clipboard.claimed)
    {
      wimp_message message;
      message.size = sizeof(wimp_full_message_release_entity);
      message.sender = 0;
      message.my_ref = 0;
      message.your_ref = 0;
      message.action = message_RELEASE_ENTITY;
      message.data.release_entity.flags = wimp_RELEASE_CLIPBOARD;
      xwimp_send_message(wimp_USER_MESSAGE, &message, wimp_BROADCAST);
    }
}

void
gdk_riscos_clipboard_free(GdkDisplay *display)
{
  GdkRiscosDisplay *rdisplay = GDK_RISCOS_DISPLAY(display);

  if (rdisplay->clipboard.claimed)
    {
      if (rdisplay->clipboard.buffer)
	free(rdisplay->clipboard.buffer);

      rdisplay->clipboard.size = 0;
      rdisplay->clipboard.max_size = 0;
      rdisplay->clipboard.claimed = FALSE;
    }
}
