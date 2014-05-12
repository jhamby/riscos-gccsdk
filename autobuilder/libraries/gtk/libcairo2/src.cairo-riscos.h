/* cairo - a vector graphics library with display and print output
 *
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * Contributor(s):
 * 		Lee Noar.
 */

#ifndef CAIRO_RISCOS_SPRITE_H
#define CAIRO_RISCOS_SPRITE_H

#include "cairo.h"

#if CAIRO_HAS_RISCOS_SPRITE_SURFACE

//#define CAIRO_RISCOS_DEBUG_MEMORY 1

#include <stdlib.h>
#include <kernel.h>

typedef struct _cairo_riscos_sprite_surface cairo_riscos_sprite_surface_t;

CAIRO_BEGIN_DECLS

cairo_surface_t *
cairo_riscos_sprite_surface_create (cairo_format_t format,
				    int width,
				    int height);

cairo_status_t
cairo_riscos_switch_output_to_surface (cairo_surface_t *surface);

cairo_status_t
cairo_riscos_switch_output_from_surface (cairo_surface_t *surface);

cairo_status_t
cairo_riscos_sprite_surface_write_to_file (cairo_surface_t *surface,
					   const char *name);

cairo_status_t
cairo_riscos_sprite_surface_get_info (cairo_surface_t *abstract_surface,
				      void **sprite_area,
				      void **sprite_pointer);

void
cairo_riscos_sprite_surface_set_size (cairo_surface_t *abstract_surface,
				      int width,
				      int height);

const _kernel_oserror *
cairo_riscos_last_oserror (cairo_bool_t reset);

cairo_status_t
cairo_riscos_error (const _kernel_oserror *err);

void *
cairo_riscos_memory_alloc (size_t size);

int
cairo_riscos_memory_free (void *addr, size_t len);

#if CAIRO_HAS_RISCOS_FONT

cairo_public cairo_font_face_t *
cairo_riscos_font_face_create (const char *name);

const char *
cairo_riscos_font_face_name (cairo_font_face_t *cface);

#endif  /* CAIRO_HAS_RISCOS_FONT */

void
cairo_riscos_finalise (void);

#ifdef CAIRO_RISCOS_DEBUG_MEMORY
void dump_memory_node_list (const char *title);
#endif

CAIRO_END_DECLS

#else  /* CAIRO_HAS_RISCOS_SPRITE_SURFACE */
# error Cairo was not compiled with support for the RISC OS sprite backend
#endif /* CAIRO_HAS_RISCOS_SPRITE_SURFACE */

#endif /* CAIRO_RISCOS_SPRITE_H */
