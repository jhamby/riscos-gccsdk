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
 * Private uninstalled header defining things local to windowing code
 */

#ifndef __GDK_PRIVATE_RISCOS_H__
#define __GDK_PRIVATE_RISCOS_H__

#include <gdk/gdkprivate.h>
#include <gdk/gdkinternals.h>
#include "gdkwindow-riscos.h"
#include "gdkdisplay-riscos.h"

#include "gdkriscoscursor.h"
#include "gdkriscosvisual.h"
#include "gdkriscoswindow.h"

#include "oslib/wimp.h"

extern GdkDisplay *_gdk_display;
extern GdkScreen *_gdk_screen;
extern GdkWindow *_gdk_root;

void _gdk_riscos_resync_windows (void);
void _gdk_riscos_windowing_init (void);

gchar * _gdk_riscos_display_manager_get_atom_name (GdkDisplayManager *manager,
						     GdkAtom atom);
void _gdk_riscos_display_manager_add_display (GdkDisplayManager *manager,
						GdkDisplay        *display);
void _gdk_riscos_display_manager_remove_display (GdkDisplayManager *manager,
						   GdkDisplay        *display);
GdkAtom _gdk_riscos_display_manager_atom_intern_static_string (GdkDisplayManager *manager,
								 const gchar *atom_name);
GdkAtom _gdk_riscos_display_manager_atom_intern (GdkDisplayManager *manager,
						   const gchar *atom_name, 
						   gboolean     only_if_exists);


void     _gdk_riscos_window_register_dnd (GdkWindow      *window);
GdkDragContext * _gdk_riscos_window_drag_begin (GdkWindow *window,
						  GdkDevice *device,
						  GList     *targets);
gboolean _gdk_riscos_window_queue_antiexpose  (GdkWindow *window,
						 cairo_region_t *area);
void     _gdk_riscos_window_translate         (GdkWindow *window,
						 cairo_region_t *area,
						 gint       dx,
						 gint       dy);
gboolean _gdk_riscos_window_get_property (GdkWindow   *window,
					    GdkAtom      property,
					    GdkAtom      type,
					    gulong       offset,
					    gulong       length,
					    gint         pdelete,
					    GdkAtom     *actual_property_type,
					    gint        *actual_format_type,
					    gint        *actual_length,
					    guchar     **data);
void _gdk_riscos_window_change_property (GdkWindow    *window,
					   GdkAtom       property,
					   GdkAtom       type,
					   gint          format,
					   GdkPropMode   mode,
					   const guchar *data,
					   gint          nelements);
void _gdk_riscos_window_delete_property (GdkWindow *window,
					   GdkAtom    property);
#if 0
gboolean _gdk_riscos_moveresize_handle_event   (GdkDisplay *display,
						  RiscosInputMsg *msg);
#endif
gboolean _gdk_riscos_moveresize_configure_done (GdkDisplay *display,
						  GdkWindow  *window);


void     _gdk_riscos_selection_window_destroyed (GdkWindow *window);
void     _gdk_riscos_window_grab_check_destroy (GdkWindow *window);

void _gdk_keymap_keys_changed     (GdkDisplay      *display);
gint _gdk_riscos_get_group_for_state (GdkDisplay      *display,
					GdkModifierType  state);
void _gdk_keymap_add_virtual_modifiers_compat (GdkKeymap       *keymap,
                                               GdkModifierType *modifiers);
gboolean _gdk_keymap_key_is_modifier   (GdkKeymap       *keymap,
					guint            keycode);

void _gdk_riscos_initialize_locale (void);

void _gdk_riscos_screen_events_init   (GdkScreen *screen);
void _gdk_riscos_events_init (GdkDisplay *display);
GdkVisual *_gdk_riscos_screen_get_system_visual (GdkScreen * screen);
gint _gdk_riscos_screen_visual_get_best_depth (GdkScreen * screen);
GdkVisualType _gdk_riscos_screen_visual_get_best_type (GdkScreen * screen);
GdkVisual *_gdk_riscos_screen_get_system_visual (GdkScreen * screen);
GdkVisual*_gdk_riscos_screen_visual_get_best (GdkScreen * screen);
GdkVisual*_gdk_riscos_screen_visual_get_best_with_depth (GdkScreen * screen,
							   gint depth);
GdkVisual*_gdk_riscos_screen_visual_get_best_with_type (GdkScreen * screen,
							  GdkVisualType visual_type);
GdkVisual*_gdk_riscos_screen_visual_get_best_with_both (GdkScreen * screen,
							  gint          depth,
							  GdkVisualType visual_type);
void _gdk_riscos_screen_query_depths  (GdkScreen * screen,
					 gint **depths,
					 gint  *count);
void _gdk_riscos_screen_query_visual_types (GdkScreen * screen,
					      GdkVisualType **visual_types,
					      gint           *count);
GList *_gdk_riscos_screen_list_visuals (GdkScreen *screen);
#if 0
void _gdk_riscos_screen_size_changed (GdkScreen *screen, 
					RiscosInputScreenResizeNotify *msg);

void _gdk_riscos_events_got_input      (GdkDisplay *display,
					  RiscosInputMsg *message);
#endif
void _gdk_riscos_screen_init_root_window (GdkScreen *screen);
void _gdk_riscos_screen_init_visuals (GdkScreen *screen);
void _gdk_riscos_display_init_dnd (GdkDisplay *display);
GdkDisplay * _gdk_riscos_display_open (const gchar *display_name);
void _gdk_riscos_display_queue_events (GdkDisplay *display);
GdkDragProtocol _gdk_riscos_window_get_drag_protocol (GdkWindow *window,
							GdkWindow **target);
GdkCursor*_gdk_riscos_display_get_cursor_for_type (GdkDisplay    *display,
						     GdkCursorType  cursor_type);
GdkCursor*_gdk_riscos_display_get_cursor_for_name (GdkDisplay  *display,
						     const gchar *name);
GdkCursor *
_gdk_riscos_display_get_cursor_for_pixbuf (GdkDisplay      *display,
					   GdkPixbuf  *pixbuf,
					   gint          x,
					   gint          y);
gboolean _gdk_riscos_display_supports_cursor_alpha (GdkDisplay *display);
gboolean _gdk_riscos_display_supports_cursor_color (GdkDisplay *display);
void _gdk_riscos_display_get_default_cursor_size (GdkDisplay *display,
						    guint       *width,
						    guint       *height);
void _gdk_riscos_display_get_maximal_cursor_size (GdkDisplay *display,
						    guint       *width,
						    guint       *height);
//void       _gdk_riscos_display_before_process_all_updates (GdkDisplay *display);
//void       _gdk_riscos_display_after_process_all_updates  (GdkDisplay *display);
void       _gdk_riscos_display_create_window_impl     (GdkDisplay    *display,
							 GdkWindow     *window,
							 GdkWindow     *real_parent,
							 GdkScreen     *screen,
							 GdkEventMask   event_mask,
							 GdkWindowAttr *attributes,
							 gint           attributes_mask);
gboolean _gdk_riscos_display_set_selection_owner (GdkDisplay *display,
						    GdkWindow  *owner,
						    GdkAtom     selection,
						    guint32     time,
						    gboolean    send_event);
GdkWindow * _gdk_riscos_display_get_selection_owner (GdkDisplay *display,
						       GdkAtom     selection);
gint _gdk_riscos_display_get_selection_property (GdkDisplay *display,
						   GdkWindow  *requestor,
						   guchar    **data,
						   GdkAtom    *ret_type,
						   gint       *ret_format);
void _gdk_riscos_display_send_selection_notify (GdkDisplay       *display,
						  GdkWindow       *requestor,
						  GdkAtom          selection,
						  GdkAtom          target,
						  GdkAtom          property, 
						  guint32          time);
void _gdk_riscos_display_convert_selection (GdkDisplay *display,
					      GdkWindow *requestor,
					      GdkAtom    selection,
					      GdkAtom    target,
					      guint32    time);
gint _gdk_riscos_display_text_property_to_utf8_list (GdkDisplay    *display,
						       GdkAtom        encoding,
						       gint           format,
						       const guchar  *text,
						       gint           length,
						       gchar       ***list);
gchar *_gdk_riscos_display_utf8_to_string_target (GdkDisplay  *display,
						    const gchar *str);
GdkKeymap* _gdk_riscos_display_get_keymap (GdkDisplay *display);
void _gdk_riscos_display_consume_all_input (GdkDisplay *display);
#if 0
RiscosInputMsg * _gdk_riscos_display_block_for_input (GdkDisplay *display,
							  char op,
							  guint32 serial,
							  gboolean remove);
#endif
/* Window methods - testing */
void     _gdk_riscos_window_sync_rendering    (GdkWindow       *window);
gboolean _gdk_riscos_window_simulate_key      (GdkWindow       *window,
						 gint             x,
						 gint             y,
						 guint            keyval,
						 GdkModifierType  modifiers,
						 GdkEventType     key_pressrelease);
gboolean _gdk_riscos_window_simulate_button   (GdkWindow       *window,
						 gint             x,
						 gint             y,
						 guint            button,
						 GdkModifierType  modifiers,
						 GdkEventType     button_pressrelease);
void _gdk_riscos_window_resize_surface        (GdkWindow *window);

void _gdk_riscos_cursor_update_theme (GdkCursor *cursor);
void _gdk_riscos_cursor_display_finalize (GdkDisplay *display);

void
_gdk_riscos_generate_button_event (GdkWindowImplRiscos *impl,
				   GdkEventType type,
				   GdkPoint *,
				   guint buttons);
guint
gdk_riscos_read_modifiers (void);

guint
gdk_riscos_translate_buttons (guint mouse_buttons);

GdkWindow *
_gdk_riscos_window_find_child (GdkWindow *window,
			       gint       x,
			       gint       y,
                               gboolean   get_toplevel);

void
gdk_riscos_window_dump_to_file (GdkWindow *window,
				const char *file);

#define GDK_SCREEN_DISPLAY(screen)    (GDK_RISCOS_SCREEN (screen)->display)
#define GDK_WINDOW_SCREEN(win)	      (GDK_WINDOW_IMPL_RISCOS (((GdkWindow *)win)->impl)->screen)
#define GDK_WINDOW_DISPLAY(win)       (GDK_RISCOS_SCREEN (GDK_WINDOW_SCREEN (win))->display)
#define GDK_WINDOW_IS_RISCOS(win)   (GDK_IS_WINDOW_IMPL_RISCOS (((GdkWindow *)win)->impl))

#endif /* __GDK_PRIVATE_RISCOS_H__ */
