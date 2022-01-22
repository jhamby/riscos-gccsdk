/*
 * gdkdisplay-riscos.h
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

#ifndef __GDK_RISCOS_DISPLAY__
#define __GDK_RISCOS_DISPLAY__

#include <stdbool.h>
#include "gdkdisplayprivate.h"
#include "oslib/wimp.h"
#include "oslib/toolbox.h"
#include "oslib/osspriteop.h"
#include "gdkmonitorprivate.h"

G_BEGIN_DECLS

typedef struct _GdkRiscosDisplay GdkRiscosDisplay;
typedef struct _GdkRiscosDisplayClass GdkRiscosDisplayClass;

typedef struct _GdkRiscosSelection GdkRiscosSelection;
typedef struct _GdkRiscosXferProgress GdkRiscosXferProgress;

#define GDK_TYPE_RISCOS_DISPLAY              (gdk_riscos_display_get_type())
#define GDK_RISCOS_DISPLAY(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_RISCOS_DISPLAY, GdkRiscosDisplay))
#define GDK_RISCOS_DISPLAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_RISCOS_DISPLAY, GdkRiscosDisplayClass))
#define GDK_IS_RISCOS_DISPLAY(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_RISCOS_DISPLAY))
#define GDK_IS_RISCOS_DISPLAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_RISCOS_DISPLAY))
#define GDK_RISCOS_DISPLAY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_RISCOS_DISPLAY, GdkRiscosDisplayClass))

struct _GdkRiscosSelection {
  void *buffer;
  size_t size;
  size_t max_size;
  bool claimed;
};

struct _GdkRiscosXferProgress
{
  size_t bytes_remaining;
  size_t bytes_transfered;
};

typedef void (*mode_change_handler_function)(void *);

struct _GdkRiscosDisplay
{
  GdkDisplay parent_instance;
  GdkScreen *default_screen;
  GdkScreen **screens;

  GdkMonitor *monitor;

  GHashTable *id_ht;
  GList *toplevels;

  GHashTable *gdkkey_hashtable;
  GHashTable *inkey_hashtable;

  GList *raw_event_handlers;
  GList *message_handlers;

  guint32 next_serial;
  guint32 last_seen_time;

  GdkKeymap *keymap;

  wimp_t task_handle;
  const char *task_name;

  wimp_pointer last_click;

  bool system_drag;

  GdkPoint last_mouse_move;

  GSList* key_down_list;

  GdkWindow *focus_window;
  GdkWindow *selection_window;
  GdkRiscosSelection selection;
  GdkRiscosSelection clipboard;
  GdkRiscosXferProgress *clipboard_xfer_progress;

  wimp_block poll_block;

  /* Mode independence.  */
  osspriteop_trans_tab *trans_table;
  os_factors render_scale;
  os_factors *render_scale_ptr;

  /* Toolbox tasks */
  toolbox_block *tb_block;
  messagetrans_control_block *tb_messages;
  char const *resource_dir_name;
};

struct _GdkRiscosDisplayClass
{
  GdkDisplayClass parent_class;
};

GType      gdk_riscos_display_get_type            (void);

GdkWindow *
gdk_riscos_window_lookup_for_display (GdkDisplay *display,
				      wimp_w window_handle);

void
_gdk_riscos_display_set_last_seen_time (GdkRiscosDisplay *display);

G_END_DECLS

#endif				/* __GDK_RISCOS_DISPLAY__ */
