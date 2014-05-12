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
 *
 *
 * Contributor(s):
 *      Lee Noar.
 */

#include "cairoint.h"
#include "cairo-riscos.h"
#include "cairo-image-surface-private.h"
#include "cairo-pattern-private.h"
#include "cairo-hash-private.h"
#include "cairo-list-inline.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <kernel.h>
#include <swis.h>

//#define DEBUG_FONTCACHE

#define CAIRO_RISCOS_FONTFACE_CACHE
#define MAX_UNUSED_FACE_ENTRIES 16

//#define DISABLE_INLINING

#ifdef DISABLE_INLINING
#define INLINE __attribute__((noinline))
#else
#define INLINE
#endif

#define FONT_ID 0x544E4F46

#define COORD12_SIZE 3
#define COORD8_SIZE 2

extern const cairo_font_face_backend_t _cairo_riscos_font_face_backend;

static const char *outline_str [] =
{
    "Outlines",
    "Outlines0"
};

#if 0
static const char *metrics_str [] =
{
    "IntMetrics",
    "IntMetric0"
};
#endif

typedef struct _font_outlines_layout font_outlines_layout_t;

struct _font_outlines_layout {
    unsigned int		ID;
    /* 0 = outlines, 1 = 1bpp, 4 = 4bpp.  */
    char			bpp;
    char			version;
    union {
	short		design_size;		// if bpp = 0
	unsigned short	flags;			// if bpp > 0
    } __attribute__((packed));
    short			bounding_box_x;
    short			bounding_box_y;
    short			bounding_box_w;
    short			bounding_box_h;
    union {
	unsigned int	chunk_file_offset[9];		// if version < 8
	struct {					// if version >= 8
	    unsigned int	chunks_file_offset;
	    int		nchunks;
	    /* Number of scaffold index entries.  */
	    int		ns;
	    int		scaffold_flags;
	    int		reserved[5];
	} __attribute__((packed));
    } __attribute__((packed));

    short			table_size;
    // Table start
    short			table_data[];
} __attribute__((packed));

typedef struct _font_metrics_layout font_metrics_layout_t;

struct _font_metrics_layout {
    char name [40];

    int x_point_multiplier; // Always 16
    int y_point_multiplier; // Always 16

    char nlo;
    char version;
    char flags;
    char nhi;
};

typedef struct _font_char_reader font_char_reader_t;

struct _font_char_reader {
    char *ptr;

    unsigned int flags;

    int design_size;
    int coord_size;

    int (*get_char_code)(font_char_reader_t *fr);
    void (*get_coord)(font_char_reader_t *fr, cairo_point_int_t *coord_ret);
};

enum segment_type {
    segment_type_terminate,
    segment_type_move,
    segment_type_line,
    segment_type_curve
};

typedef struct _cairo_riscos_segment cairo_riscos_segment_t;
typedef struct _cairo_riscos_font_face cairo_riscos_font_face_t;
typedef struct _cairo_riscos_glyph cairo_riscos_glyph_t;

struct _cairo_riscos_segment {
    int type;

    /* move_to and line_to use the first element only, while curve_to uses
     * all three.  */
    cairo_point_t point[3];

    cairo_point_double_t orig_point[3];
};

struct _cairo_riscos_glyph {
    cairo_hash_entry_t base;

    int code;

    /* Bounding box.  */
    cairo_box_t bounds;

    cairo_rectangle_int_t orig_bounds;

    cairo_bool_t stroke_paths;
    cairo_bool_t composite;

    int nsegments;

    /* Must be last member.  */
    cairo_riscos_segment_t segments[];
};

struct _cairo_riscos_font_face {
    cairo_font_face_t base;
    cairo_list_t link;

    int design_size;

    cairo_hash_table_t *glyph_hash_map;

    cairo_box_t bounds;

    char *name;
};

typedef struct _cairo_riscos_scaled_font {
    cairo_scaled_font_t base;

} cairo_riscos_scaled_font_t;

#ifdef CAIRO_RISCOS_FONTFACE_CACHE

typedef struct _cairo_fontface_cache {
    cairo_hash_table_t *used_face_ht;
    int used_face_count;

    /* New entries are added to the start of the list.  */
    cairo_list_t unused_face_list;
    int unused_face_count;
} cairo_fontface_cache;

static cairo_fontface_cache *fontface_cache = NULL;

#endif

typedef unsigned font_handle;

static cairo_status_t INLINE
_cairo_riscos_font_face_scaled_font_create (void *abstract_face,
					    const cairo_matrix_t *font_matrix,
					    const cairo_matrix_t *ctm,
					    const cairo_font_options_t *options,
					    cairo_scaled_font_t **font);

static const _kernel_oserror *
riscos_find_font (const char *__identifier,
		  int __x_size,
		  int __y_size,
		  font_handle *handle)
{
    register const char *identifier __asm ("r1") = __identifier;
    register int x_size __asm ("r2") = __x_size;
    register int y_size __asm ("r3") = __y_size;
    register const _kernel_oserror *err __asm ("r0");

    __asm__ volatile ("	MOV	r4, #0;\n"
		      "	MOV	r5, #0;\n"
		      "	SWI	%[SWI_XFont_FindFont];\n"
		      "	STRVC	r0, %[font_handle];\n"
		      "	MOVVC	r0, #0;\n"
		      : "=r" (err),
			[font_handle] "=m" (*handle)
		      :	"r" (identifier),
			"r" (x_size),
			"r" (y_size),
			[SWI_XFont_FindFont] "i" (Font_FindFont | XOS_Bit)
		      : "r4", "r5", "cc", "memory");
    return err;
}

static void
riscos_lose_font (font_handle __handle)
{
    register font_handle handle __asm ("r0") = __handle;

    __asm__ volatile ("	SWI	%[SWI_XFont_LoseFont];\n"
		      :
		      :	"r" (handle),
			[SWI_XFont_LoseFont] "i" (Font_LoseFont | XOS_Bit)
		      : "cc");
}

static const _kernel_oserror *
riscos_paint_font (font_handle __handle,
		   const char *__str,
		   int __x,
		   int __y)
{
    register font_handle handle __asm ("r0") = __handle;
    register const char *str __asm ("r1") = __str;
    register int x __asm ("r3") = __x;
    register int y __asm ("r4") = __y;
    register const _kernel_oserror *err __asm ("r0");

    __asm__ volatile ("	MOV	r2, #0x100;\n"
		      "	ADD	r2, r2, #0x90;\n"
		      "	MOV	r7, #1;\n"
		      "	SWI	%[SWI_Font_Paint];\n"
		      "	MOVVC	r0, #0;\n"
		      : "=r" (err)
		      : "r" (handle),
			"r" (str),
			"r" (x),
			"r" (y),
		        [SWI_Font_Paint] "i" (Font_Paint | XOS_Bit)
		      : "r2", "r7", "cc");
		      /* r5 & r6 deliberately left out of clobber list. They're preserved, so
		         the compiler should be able to use them as it sees fit.  */
    return err;
}

static int
get_char8_code (font_char_reader_t *fr)
{
    return *fr->ptr++;
}

static int
get_char16_code (font_char_reader_t *fr)
{
    int c= *fr->ptr++;

    return c | ((*fr->ptr++) << 8);
}

static void
get_coord8 (font_char_reader_t *fr,
	    cairo_point_int_t *coord_ret)
{
    char *ptr = fr->ptr;

    int x = *ptr++;
    int y = *ptr++;

    fr->ptr = ptr;

    // sign extend
    coord_ret->x = (x << 24) >> 24;
    coord_ret->y = (y << 24) >> 24;
}

static void
get_coord12 (font_char_reader_t *fr,
	     cairo_point_int_t *coord_ret)
{
    char *ptr = fr->ptr;

    int x = *ptr++;
    int b = *ptr++;
    int y;

    x |= (b & 0xF) << 8;
    y = (b & 0xF0) >> 4;
    y |= (*ptr++) << 4;

    fr->ptr = ptr;

    // sign extend
    coord_ret->x = (x << 20) >> 20;
    coord_ret->y = (y << 20) >> 20;
}

static void INLINE
get_bounding_box (font_char_reader_t *fcr,
		  cairo_rectangle_int_t *box_ret)
{
    cairo_point_int_t point;

    fcr->get_coord (fcr, &point);
    box_ret->x = point.x;
    box_ret->y = point.y;

    fcr->get_coord (fcr, &point);
    box_ret->width = point.x;
    box_ret->height = point.y;
}

static void INLINE
font_char_reader_init (font_char_reader_t *fcr,
		       char *ptr,
		       int design_size)
{
    fcr->ptr = ptr;
    fcr->flags = (unsigned int)*fcr->ptr++;
    fcr->design_size = design_size;

    if (fcr->flags & (1 << 0)) {
	fcr->get_coord = get_coord12;
	fcr->coord_size = COORD12_SIZE;
    }
    else {
	fcr->get_coord = get_coord8;
	fcr->coord_size = COORD8_SIZE;
    }
    
    fcr->get_char_code = ((fcr->flags & ((1 << 3) | (1 << 6))) == ((1 << 3) | (1 << 6))) ?
			 get_char16_code :
			 get_char8_code;
}

static void INLINE
_cairo_riscos_scaled_font_fini (void *abstract_font)
{
}

static void
_cairo_riscos_font_face_remove_glyph (void *entry, void *closure)
{
    cairo_riscos_glyph_t *glyph = entry;
    cairo_riscos_font_face_t *font_face = closure;

    _cairo_hash_table_remove (font_face->glyph_hash_map,
			      &glyph->base);
    free (glyph);
}

static cairo_int_status_t INLINE
_cairo_riscos_scaled_font_init_glyph_metrics (cairo_riscos_scaled_font_t *scaled_font,
					      cairo_scaled_glyph_t *scaled_glyph)
{
    const cairo_riscos_glyph_t *glyph;
    cairo_riscos_glyph_t key;
    cairo_text_extents_t extents = {0, 0, 0, 0, 0, 0};
    cairo_riscos_font_face_t *font_face;
    double xmin, ymin, xmax, ymax;

    font_face = (cairo_riscos_font_face_t *)scaled_font->base.font_face;

    key.base.hash = key.code = _cairo_scaled_glyph_index(scaled_glyph);
    if (key.code == 32) {
	/* Use the letter 'l' to size up the space character.  */
	key.base.hash = key.code = 'l';
    }

    glyph = _cairo_hash_table_lookup (font_face->glyph_hash_map, &key.base);
    if (unlikely (glyph == NULL))
	goto FAIL;

    xmin = _cairo_fixed_to_double(glyph->bounds.p1.x);
    ymin = _cairo_fixed_to_double(glyph->bounds.p1.y);
    xmax = _cairo_fixed_to_double(glyph->bounds.p2.x);
    ymax = _cairo_fixed_to_double(glyph->bounds.p2.y);

    extents.x_bearing = xmin;
    extents.y_bearing = -ymin;
    extents.width = xmax - xmin;
    extents.height = -(ymax - ymin);
    extents.x_advance = xmax + xmin;
    extents.y_advance = 0;
    
#if 0
    printf ("[0x%04x] extents: bearings: %f %f dim: %f %f adv: %f\n\n", key.code,
	     extents.x_bearing, extents.y_bearing, extents.width, extents.height, extents.x_advance);
#endif

FAIL:
    _cairo_scaled_glyph_set_metrics (scaled_glyph, &scaled_font->base, &extents);

    return CAIRO_STATUS_SUCCESS;
}

static cairo_t * INLINE
_cairo_riscos_scaled_font_create_glyph_context (const cairo_riscos_scaled_font_t *scaled_font,
						cairo_surface_t *glyph_surface)
{
    cairo_t *cr;

    cr = cairo_create (glyph_surface);

    cairo_set_font_size (cr, 1.0);
    cairo_set_font_options (cr, &scaled_font->base.options);
    cairo_set_source_rgb (cr, 1., 1., 1.);

    /* RISC OS outline fonts need to be filled with the even-odd rule.  */
    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);

    return cr;
}

static cairo_status_t INLINE
_cairo_riscos_render_glyph (cairo_surface_t *surface,
			    cairo_riscos_scaled_font_t *scaled_font,
			    const cairo_riscos_glyph_t *glyph,
			    cairo_t *cr, int width, int height)
{
    int i;
    double x1,y1,x2,y2,x3,y3;
    double dwidth, dheight;
#if 0
    printf ("extents: asc/des: %f %f height: %f adv: %f %f\n\n",
	     scaled_font->base.extents.ascent,
	     scaled_font->base.extents.descent,
	     scaled_font->base.extents.height,
	    scaled_font->base.extents.max_x_advance,
	    scaled_font->base.extents.max_y_advance);
    printf ("fs_extents: asc/des: %f %f height: %f adv: %f %f\n\n",
	     scaled_font->base.fs_extents.ascent,
	     scaled_font->base.fs_extents.descent,
	     scaled_font->base.fs_extents.height,
	     scaled_font->base.fs_extents.max_x_advance,
	     scaled_font->base.fs_extents.max_y_advance);
#endif

    dwidth = _cairo_fixed_to_double(glyph->bounds.p2.x/* - glyph->bounds.p1.x*/);
    dheight = _cairo_fixed_to_double(glyph->bounds.p2.y + glyph->bounds.p1.y);

    cairo_save (cr);

    if (!_cairo_matrix_is_scale_0 (&scaled_font->base.scale)) {
	cairo_matrix_t scale;
#if 1
	cairo_matrix_init (&scale, 1, 0, 0, -1, 0, height);
	cairo_matrix_translate (&scale, width / 2, height / 2);
	cairo_matrix_multiply (&scale, &scaled_font->base.scale, &scale);
	scale.yx = -scale.yx;
	scale.xy = -scale.xy;
	cairo_matrix_translate (&scale, -dwidth / 2, -dheight / 2);
#else
	cairo_matrix_init (&scale, 1, 0, 0, -1, 0, dheight);
//	cairo_matrix_init (&scale, 1, 0, 0, 1, 0, 0);//dheight);
	cairo_matrix_translate (&scale, dwidth / 2, dheight / 2);
	cairo_matrix_multiply (&scale, &scale, &scaled_font->base.scale);
	cairo_matrix_translate (&scale, -dwidth / 2, -dheight / 2);
#endif
//	scale = scaled_font->base.scale;
//	scale.x0 = scale.y0 = 0.;
	cairo_set_matrix (cr, &scale);
    }
#if 0
{
  cairo_matrix_t matrix;
  cairo_get_matrix (cr, &matrix);
printf ("[%s:%s:%d]\n",
	__FILE__,__func__,__LINE__);
printf (" render matrix\n");
printf ("  %f  %f\n", matrix.xx,matrix.yx);
printf ("  %f  %f\n", matrix.xy,matrix.yy);
printf ("  %f  %f\n", matrix.x0,matrix.y0);
printf (" scaled_font->base matrix\n");
printf ("  %f  %f\n", scaled_font->base.scale.xx,scaled_font->base.scale.yx);
printf ("  %f  %f\n", scaled_font->base.scale.xy,scaled_font->base.scale.yy);
printf ("  %f  %f\n", scaled_font->base.scale.x0,scaled_font->base.scale.y0);
}
#endif
    for (i = 0; i < glyph->nsegments; i++) {
	const cairo_riscos_segment_t *segment = &glyph->segments[i];

        switch (segment->type)
	{
	case segment_type_move:
	    x1 = segment->orig_point[0].x;
	    y1 = segment->orig_point[0].y;
	    cairo_move_to (cr, x1, y1);
	    continue;
	case segment_type_line:
	    x1 = segment->orig_point[0].x;
	    y1 = segment->orig_point[0].y;
	    cairo_line_to (cr, x1, y1);
	    continue;
	case segment_type_curve:
	    x1 = segment->orig_point[0].x;
	    y1 = segment->orig_point[0].y;
	    x2 = segment->orig_point[1].x;
	    y2 = segment->orig_point[1].y;
	    x3 = segment->orig_point[2].x;
	    y3 = segment->orig_point[2].y;
	    cairo_curve_to (cr, x1, y1, x2, y2, x3, y3);
	    continue;
	}
    }

    cairo_restore (cr);

    cairo_set_line_width (cr, 1);
    cairo_fill (cr);

    return CAIRO_STATUS_SUCCESS;
}

static cairo_int_status_t INLINE
_cairo_riscos_scaled_font_init_glyph_surface (cairo_riscos_scaled_font_t *scaled_font,
					      cairo_scaled_glyph_t *scaled_glyph)
{
#if 1
    const cairo_matrix_t *scale_matrix;
    cairo_riscos_font_face_t *font_face;
    cairo_surface_t *surface;
    int x1, y1, x2, y2;
    int width, height;
    cairo_t *cr;
    cairo_riscos_glyph_t key;
    const cairo_riscos_glyph_t *glyph;
    cairo_status_t status = CAIRO_STATUS_SUCCESS;

    scale_matrix = &scaled_font->base.scale;
    font_face = (cairo_riscos_font_face_t *) scaled_font->base.font_face;

    key.base.hash = key.code = _cairo_scaled_glyph_index(scaled_glyph);
    glyph = _cairo_hash_table_lookup (font_face->glyph_hash_map, &key.base);
    if (unlikely (glyph == NULL))
	return CAIRO_STATUS_INVALID_INDEX;

    x1 = _cairo_fixed_integer_floor (scaled_glyph->bbox.p1.x);
    y1 = _cairo_fixed_integer_floor (scaled_glyph->bbox.p1.y);
    x2 = _cairo_fixed_integer_ceil (scaled_glyph->bbox.p2.x);
    y2 = _cairo_fixed_integer_ceil (scaled_glyph->bbox.p2.y);

    width = x2 - x1;
    height = y2 - y1;

    surface = cairo_image_surface_create (CAIRO_FORMAT_A8,
					  width, height);
    if (unlikely(surface->status))
	return surface->status;

    cr = _cairo_riscos_scaled_font_create_glyph_context (scaled_font,
							 surface);
    if (unlikely (cairo_status (cr))) {
	cairo_surface_destroy (surface);
	return cairo_status (cr);
    }
//printf ("* [%s:%s:%d] - height = %d, char = '%c'\n",__FILE__,__func__,__LINE__, height,key.code);

    status = _cairo_riscos_render_glyph (surface, scaled_font, glyph, cr, width, height);
#if 0
{
static int count = 0;
char name[20];
sprintf (name, "ram:$.surface%d\n",count++);
cairo_surface_write_to_png (surface, name);
}
#endif

    cairo_surface_set_device_offset (surface, -x1, -y1);
    _cairo_scaled_glyph_set_surface (scaled_glyph,
                                     &scaled_font->base,
				     (cairo_image_surface_t *)surface);
    return status;
#else
    cairo_riscos_font_face_t *font_face = (cairo_riscos_font_face_t *) scaled_font->base.font_face;
    const cairo_matrix_t *scale_matrix = &scaled_font->base.scale;
    cairo_status_t status;
    cairo_surface_t *surface;
    int x1, y1, x2, y2;
    int width, height;
    font_handle handle;
    char buffer[2];

    printf ("%s:%s:%d: xx = %f, yy = %f\n",__FILE__,__func__,__LINE__,scale_matrix->xx,scale_matrix->yy);
    x1 = _cairo_fixed_integer_floor (scaled_glyph->bbox.p1.x);
    y1 = _cairo_fixed_integer_floor (scaled_glyph->bbox.p1.y);
    x2 = _cairo_fixed_integer_ceil (scaled_glyph->bbox.p2.x);
    y2 = _cairo_fixed_integer_ceil (scaled_glyph->bbox.p2.y);
#if 0
    width = _cairo_fixed_mul_div (x2 - x1,
				  _cairo_fixed_from_double (scale_matrix->xx),
				  1/*_cairo_fixed_from_double (font_face->design_size)*/);
    height = _cairo_fixed_mul_div (y2 - y1,
				  _cairo_fixed_from_double (scale_matrix->yy),
				  1/*_cairo_fixed_from_double (font_face->design_size)*/);
    width = _cairo_fixed_integer_ceil (width);
    height = _cairo_fixed_integer_ceil (height);
#endif
    width = x2 - x1;
    height = y2 - y1;
    
    printf ("width = %d, height = %d\n", width, height);
#if 0
    surface = cairo_riscos_sprite_surface_create (CAIRO_FORMAT_RGB24,
						  width, height);
    _cairo_scaled_glyph_set_surface (scaled_glyph,
                                     &scaled_font->base,
				     (cairo_image_surface_t *)surface);
    return;
#else
    surface = cairo_riscos_sprite_surface_create (CAIRO_FORMAT_ARGB32,
						  width, height);
    if (unlikely (surface->status)) {
	status = surface->status;
	goto CLEANUP_BEFORE_FONT;
    }
/*    status = _cairo_surface_paint (surface, CAIRO_OPERATOR_SOURCE,
				   &_cairo_pattern_white.base, NULL);
    if (status)
	goto CLEANUP_BEFORE_FONT;*/

    status = cairo_riscos_error (riscos_find_font (font_face->name,
						   width << 4,
						   height << 4,
						   &handle));
    if (unlikely (status))
	goto CLEANUP_BEFORE_SWITCH;

    status = cairo_riscos_switch_output_to_surface (surface);
    if (unlikely (status))
	goto CLEANUP_BEFORE_SWITCH;

    buffer[0] = _cairo_scaled_glyph_index(scaled_glyph);
    buffer[1] = '\0';
    status = cairo_riscos_error (riscos_paint_font (handle, buffer, 0, 0));
    if (unlikely (status))
	goto CLEANUP_AFTER_SWITCH;

    cairo_surface_set_device_offset (surface, 0, height);

    _cairo_scaled_glyph_set_surface (scaled_glyph,
                                     &scaled_font->base,
				     (cairo_image_surface_t *)surface);
#endif
/*    printf (" bbox (%f, %f) (%f, %f)\n",
	    _cairo_fixed_to_double(scaled_glyph->bbox.p1.x),
	    _cairo_fixed_to_double(scaled_glyph->bbox.p1.y),
	    _cairo_fixed_to_double(scaled_glyph->bbox.p2.x),
	    _cairo_fixed_to_double(scaled_glyph->bbox.p2.y));*/
CLEANUP_AFTER_SWITCH:
    cairo_riscos_switch_output_from_surface (surface);
CLEANUP_BEFORE_SWITCH:
    riscos_lose_font (handle);
CLEANUP_BEFORE_FONT:
cairo_riscos_sprite_surface_write_to_file (surface, "ram:$.d");
    return status;
#endif
}

static cairo_int_status_t INLINE
_cairo_riscos_scaled_font_init_glyph_path (cairo_riscos_scaled_font_t *scaled_font,
					   cairo_scaled_glyph_t *scaled_glyph)
{
    cairo_int_status_t status = CAIRO_STATUS_SUCCESS;
    const cairo_matrix_t *scale_matrix = &scaled_font->base.scale;
    cairo_path_fixed_t *path;
    const cairo_riscos_glyph_t *glyph_data;
    cairo_riscos_glyph_t key;
    cairo_riscos_font_face_t *face = (cairo_riscos_font_face_t *) scaled_font->base.font_face;
    int i;

    path = _cairo_path_fixed_create ();
    if (unlikely (path == NULL))
	return _cairo_error (CAIRO_STATUS_NO_MEMORY);

    key.base.hash = key.code = _cairo_scaled_glyph_index(scaled_glyph);
    glyph_data = _cairo_hash_table_lookup (face->glyph_hash_map, &key.base);
    if (unlikely (glyph_data == NULL))
	return CAIRO_STATUS_INVALID_INDEX;

    for (i = 0; i < glyph_data->nsegments; ++i) {
	const cairo_riscos_segment_t *segment = &glyph_data->segments[i];

	switch (segment->type)
	{
	case segment_type_move:
	    status = _cairo_path_fixed_move_to (path,
						segment->point[0].x,
						-segment->point[0].y);
	    break;
	case segment_type_line:
	    status = _cairo_path_fixed_line_to (path,
						segment->point[0].x,
						-segment->point[0].y);
	    break;
	case segment_type_curve:
	    status = _cairo_path_fixed_curve_to (path,
						 segment->point[0].x,
						 -segment->point[0].y,
						 segment->point[1].x,
						 -segment->point[1].y,
						 segment->point[2].x,
						 -segment->point[2].y);
	    break;
	}

	if (unlikely (status))
	    goto CLEANUP_PATH;
    }

    status = _cairo_path_fixed_close_path (path);

    _cairo_path_fixed_transform (path, scale_matrix);

    _cairo_scaled_glyph_set_path (scaled_glyph,
				  &scaled_font->base,
				  path);

CLEANUP_PATH:
    if (status != CAIRO_INT_STATUS_SUCCESS)
	_cairo_path_fixed_destroy (path);

    return status;
}

static cairo_int_status_t INLINE
_cairo_riscos_scaled_glyph_init (void			 *abstract_font,
				 cairo_scaled_glyph_t	 *scaled_glyph,
				 cairo_scaled_glyph_info_t  info)
{
    cairo_int_status_t status = CAIRO_STATUS_SUCCESS;
    cairo_riscos_scaled_font_t *scaled_font = abstract_font;

    if (info & CAIRO_SCALED_GLYPH_INFO_METRICS)
	status = _cairo_riscos_scaled_font_init_glyph_metrics (scaled_font, scaled_glyph);

    if (!status && (info & CAIRO_SCALED_GLYPH_INFO_SURFACE))
	status = _cairo_riscos_scaled_font_init_glyph_surface (scaled_font, scaled_glyph);

    if (!status && (info & CAIRO_SCALED_GLYPH_INFO_PATH))
	status = _cairo_riscos_scaled_font_init_glyph_path (scaled_font, scaled_glyph);

    return status;
}

static unsigned long
_cairo_riscos_ucs4_to_index (void	    *abstract_font,
			     uint32_t  ucs4)
{
    return ucs4;
}

static const cairo_scaled_font_backend_t _cairo_riscos_scaled_font_backend = {
    CAIRO_FONT_TYPE_RISCOS,
    _cairo_riscos_scaled_font_fini,
    _cairo_riscos_scaled_glyph_init,
    NULL,
    _cairo_riscos_ucs4_to_index,
    NULL,
    NULL,
    NULL,
};

static cairo_status_t
_cairo_riscos_font_face_create_for_toy (cairo_toy_font_face_t   *toy_face,
					cairo_font_face_t      **font_face)
{
    return CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED;
}

static cairo_bool_t
_cairo_riscos_glyphs_equal (const void *_a, const void *_b)
{
    const cairo_riscos_glyph_t *a = _a;
    const cairo_riscos_glyph_t *b = _b;

    return a->code == b->code;
}

static cairo_status_t INLINE
_cairo_riscos_font_face_scaled_font_create (void *abstract_face,
					    const cairo_matrix_t *font_matrix,
					    const cairo_matrix_t *ctm,
					    const cairo_font_options_t *options,
					    cairo_scaled_font_t **font)
{
    cairo_status_t status = CAIRO_STATUS_SUCCESS;
    cairo_riscos_font_face_t *font_face = abstract_face;
    cairo_riscos_scaled_font_t *scaled_font;
    cairo_font_extents_t fs_metrics;

    scaled_font = malloc (sizeof (cairo_riscos_scaled_font_t));
    if (unlikely (scaled_font == NULL))
	return _cairo_error (CAIRO_STATUS_NO_MEMORY);

    status = _cairo_scaled_font_init (&scaled_font->base,
				      &font_face->base,
				      font_matrix, ctm, options,
				      &_cairo_riscos_scaled_font_backend);
    if (unlikely (status)) {
	free (scaled_font);
	return status;
    }

    fs_metrics.ascent = _cairo_fixed_to_double (font_face->bounds.p2.y);
    fs_metrics.descent = _cairo_fixed_to_double (font_face->bounds.p1.y);
    fs_metrics.height = _cairo_fixed_to_double (font_face->bounds.p2.y - font_face->bounds.p1.y);
    fs_metrics.max_x_advance = _cairo_fixed_to_double (font_face->bounds.p2.x + font_face->bounds.p1.x);
    fs_metrics.max_y_advance = 0;
    status = _cairo_scaled_font_set_metrics (&scaled_font->base, &fs_metrics);
    if (unlikely (status)) {
	free (scaled_font);
	return status;
    }

    *font = &scaled_font->base;

    return CAIRO_STATUS_SUCCESS;
}

static int INLINE
_cairo_riscos_segment_array_size (font_char_reader_t *fcr)
{
    int count = 0;
    char *ptr = fcr->ptr;

    for (;;) {
	int type = (*ptr++) & 3;

	switch (type)
	{
	case segment_type_terminate:
	    break;
	case segment_type_move:
	case segment_type_line:
	    count++;
	    ptr += fcr->coord_size;
	    continue;
	case segment_type_curve:
	    count++;
	    ptr += fcr->coord_size * 3;
	    continue;
	}
	break;
    }

    return count;
}

static void __attribute__((noinline))
adjust_bounds (const cairo_point_t *point,
	       cairo_box_t *bounds)
{
    if (point->x < bounds->p1.x)
	bounds->p1.x = point->x;
    if (point->x > bounds->p2.x)
	bounds->p2.x = point->x;
    if (point->y < bounds->p1.y)
	bounds->p1.y = point->y;
    if (point->y > bounds->p2.y)
	bounds->p2.y = point->y;
}

static void INLINE
_cairo_riscos_check_bounding_box (cairo_riscos_glyph_t *glyph)
{
    int seg_count, i;
    cairo_riscos_segment_t *segment;

    seg_count = glyph->nsegments;
    segment = glyph->segments;

    while (seg_count--) {
	switch (segment->type)
	{
	case segment_type_move:
	case segment_type_line:
	    adjust_bounds (&segment->point[0], &glyph->bounds);
	    break;
	case segment_type_curve:
	    for (i = 0; i < 3; ++i)
		adjust_bounds (&segment->point[i], &glyph->bounds);
	    break;
	}

	segment++;
    }
}

static cairo_status_t INLINE
cairo_riscos_read_glyph (font_char_reader_t *fcr, cairo_riscos_glyph_t **glyph_ret)
{
    cairo_rectangle_int_t bounds;
    cairo_riscos_glyph_t *glyph;
    int segment_array_count;
    int segment_idx = 0;
    const double design_size = (double)fcr->design_size;

#if 0
    /* The code below is how the PRMs describe it, but the font file data doesn't seem
     * to match despite the relevant flags being set.  */

    /* Does a composite characater follow?  */
    if (fcr->flags & (1 << 4)) {
	fcr->get_char_code (fcr);
    }

    /* Does a composite accent character follow?  */
    if (fcr->flags & (1 << 5)) {
	int x, y;

	fcr->get_char_code (fcr);
	fcr->get_coord (fcr, &x, &y);
    }
#endif

    if ((fcr->flags & (1 << 4)) == 0 && (fcr->flags & (1 << 5)) == 0)
	get_bounding_box (fcr, &bounds);

    segment_array_count = _cairo_riscos_segment_array_size (fcr);

    glyph = malloc (sizeof (cairo_riscos_glyph_t) +
			segment_array_count * sizeof (cairo_riscos_segment_t));
    if (unlikely (glyph == NULL))
	return CAIRO_STATUS_NO_MEMORY;

    glyph->bounds.p1.x = _cairo_fixed_from_double ((double)bounds.x / design_size);
    glyph->bounds.p1.y = _cairo_fixed_from_double ((double)bounds.y / design_size);
    glyph->bounds.p2.x = glyph->bounds.p1.x + _cairo_fixed_from_double ((double)bounds.width / design_size);
    glyph->bounds.p2.y = glyph->bounds.p1.y + _cairo_fixed_from_double ((double)bounds.height / design_size);

    glyph->orig_bounds.x = bounds.x;
    glyph->orig_bounds.y = bounds.y;
    glyph->orig_bounds.width = bounds.width;
    glyph->orig_bounds.height = bounds.height;

    glyph->nsegments = segment_array_count;

    for (;;) {
        int byte = *fcr->ptr++;
	int type = byte & 3, i;
	cairo_riscos_segment_t *segment;
	cairo_point_int_t point;

	if (type == segment_type_terminate)
	{
	    int terminator_byte = *fcr->ptr++;
	    glyph->stroke_paths = (terminator_byte & (1 << 2)) != 0;
	    glyph->composite = (terminator_byte & (1 << 3)) != 0;
	    break;
	}

	segment = &glyph->segments[segment_idx++];

	segment->type = type;

	switch (type)
	{
	case segment_type_move:
	case segment_type_line:
	    fcr->get_coord (fcr, &point);
	    segment->point[0].x = _cairo_fixed_from_double ((double)point.x / design_size);
	    segment->point[0].y = _cairo_fixed_from_double ((double)point.y / design_size);

	    segment->orig_point[0].x = (double)point.x / design_size;
	    segment->orig_point[0].y = (double)point.y / design_size;
	    continue;
	case segment_type_curve:
	    for (i = 0; i < 3; ++i)
	    {
		fcr->get_coord (fcr, &point);
		segment->point[i].x = _cairo_fixed_from_double ((double)point.x / design_size);
		segment->point[i].y = _cairo_fixed_from_double ((double)point.y / design_size);

		segment->orig_point[i].x = (double)point.x / design_size;
		segment->orig_point[i].y = (double)point.y / design_size;
	    }
	    continue;
	}
    }

    _cairo_riscos_check_bounding_box (glyph);

    *glyph_ret = glyph;

    return CAIRO_STATUS_SUCCESS;
}

static FILE * INLINE
open_file (const char *name,
	   const char *leaf_names_array[],
	   int array_size)
{
    static const char font_path_str [] = "Font:";
    FILE *f = NULL;
    char filename [PATH_MAX];
    int i, name_len = strlen (name);

    if (sizeof (font_path_str) + name_len + 20 > PATH_MAX)
	return NULL;

    strcpy (filename, font_path_str);
    strcpy (filename + sizeof (font_path_str) - 1, name);
    filename [sizeof (font_path_str) - 1 + name_len] = '.';

    for (i = 0; i < array_size; ++i) {
	strcpy (filename + sizeof (font_path_str) + name_len, leaf_names_array[i]);
	if ((f = fopen (filename, "r")) != NULL)
	    return f;
    }

    return NULL;
}

#if 0
static cairo_status_t INLINE
_cairo_riscos_read_font_metrics (cairo_riscos_font_face_t *font_face,
				 font_metrics_layout_t *font_file)
{
    cairo_status_t status = CAIRO_STATUS_SUCCESS;

    printf ("defined characters = %d\n", font_file->nhi * 256 + font_file->nlo);
    printf ("version = %d\n", font_file->version);
    printf ("flags = %X\n", font_file->flags);

    return status;
}
#endif

static cairo_status_t INLINE
_cairo_riscos_read_font_outlines (cairo_riscos_font_face_t *font_face,
				  font_outlines_layout_t *font_file)
{
    cairo_status_t status = CAIRO_STATUS_SUCCESS;
    int nchunks;
    unsigned int *chunk_table;
    int chunk_number;

    if (font_file->ID != FONT_ID) {
	return CAIRO_STATUS_FONT_TYPE_MISMATCH;
    }

    font_face->design_size = font_file->design_size;
    font_face->bounds.p1.x = _cairo_fixed_from_double ((double)font_file->bounding_box_x / font_face->design_size);
    font_face->bounds.p1.y = _cairo_fixed_from_double ((double)font_file->bounding_box_y / font_face->design_size);
    font_face->bounds.p2.x = font_face->bounds.p1.x + _cairo_fixed_from_double ((double)font_file->bounding_box_w / font_face->design_size);
    font_face->bounds.p2.y = font_face->bounds.p1.y + _cairo_fixed_from_double ((double)font_file->bounding_box_h / font_face->design_size);
/*printf (">> FACE BOUNDS: %f, %f, %f, %f\n",_cairo_fixed_to_double(font_face->bounds.p1.x),
				      _cairo_fixed_to_double(font_face->bounds.p1.y),
				      _cairo_fixed_to_double(font_face->bounds.p2.x),
				      _cairo_fixed_to_double(font_face->bounds.p2.y));*/

    if (font_file->version < 8) {
	chunk_table = (unsigned int *)font_file->chunk_file_offset;
	nchunks = 8;
    }
    else {
	chunk_table = (unsigned int *)((int)font_file + font_file->chunks_file_offset);
	nchunks = font_file->nchunks;
    }

    for (chunk_number = 0; chunk_number < nchunks; chunk_number++) {
	unsigned int *chunk = (unsigned int *)((int)font_file + chunk_table[chunk_number]);
	int chunk_char_index;

	/* If chunk[n+1]=chunk[n], then chunk n is null. PRM V4,P477.
	   Remember that there is an extra entry at the end of the table
	   containing the file size.  */
	if (chunk_table[chunk_number + 1] == chunk_table[chunk_number])
	    continue;

	if (font_file->version >= 7)
	    chunk++;	// skip flags

	for (chunk_char_index = 0; chunk_char_index < 32; chunk_char_index++) {
	    font_char_reader_t reader;
	    cairo_riscos_glyph_t *glyph;

	    if (chunk[chunk_char_index] == 0)
		continue;

	    font_char_reader_init (&reader,
				   (char *)chunk + chunk[chunk_char_index],
				   font_file->design_size);

	    /* Only interested in outlines, not bitmaps.  */
	    if ((reader.flags & (1 << 3)) == 0)
		continue;

	    status = cairo_riscos_read_glyph (&reader, &glyph);
	    if (unlikely (status))
		goto EXIT;

	    glyph->code = glyph->base.hash = chunk_number * 32 + chunk_char_index;
	    status = _cairo_hash_table_insert (font_face->glyph_hash_map,
					      &glyph->base);
	    if (unlikely (status))
		goto EXIT;
	}
    }

EXIT:
    return status;
}

static cairo_status_t INLINE
_cairo_riscos_read_font (cairo_riscos_font_face_t *font_face,
			 const char *name)
{
    cairo_status_t status = CAIRO_STATUS_SUCCESS;
    size_t file_length;
    font_outlines_layout_t *font_outlines_file = NULL;
//    font_metrics_layout_t *font_metrics_file = NULL;
    FILE *f;
    
    f = open_file (name,
		   outline_str,
		   sizeof (outline_str) / sizeof (char *));
    if (unlikely (f == NULL))
	return CAIRO_STATUS_FILE_NOT_FOUND;

    fseek (f, 0, SEEK_END);
    file_length = ftell (f);
    fseek (f, 0, SEEK_SET);

    font_outlines_file = cairo_riscos_memory_alloc (file_length);
    if (unlikely (font_outlines_file == NULL)) {
        status = CAIRO_STATUS_NO_MEMORY;
	goto CLEANUP_FILE;
    }

    if (unlikely (fread (font_outlines_file, 1, file_length, f) != file_length)) {
	status = CAIRO_STATUS_READ_ERROR;
	goto CLEANUP_FILE;
    }

    fclose (f);
    f = NULL;

    status = _cairo_riscos_read_font_outlines (font_face, font_outlines_file);
    cairo_riscos_memory_free (font_outlines_file, file_length);
#if 0
    if (unlikely (status))
	return status;

    f = open_file (name,
		   metrics_str,
		   sizeof (metrics_str) / sizeof (char *));
    if (unlikely (f == NULL))
	return CAIRO_STATUS_FILE_NOT_FOUND;

    fseek (f, 0, SEEK_END);
    file_length = ftell (f);
    fseek (f, 0, SEEK_SET);

    font_metrics_file = cairo_riscos_memory_alloc (file_length);
    if (unlikely (font_metrics_file == NULL)) {
        status = CAIRO_STATUS_NO_MEMORY;
	goto CLEANUP_FILE;
    }

    if (unlikely (fread (font_metrics_file, 1, file_length, f) != file_length)) {
	status = CAIRO_STATUS_READ_ERROR;
	goto CLEANUP_FILE;
    }

    status = _cairo_riscos_read_font_metrics (font_face, font_metrics_file);
    cairo_riscos_memory_free (font_metrics_file, file_length);
#endif
CLEANUP_FILE:
    if (f)
	fclose (f);

    return status;
}

#ifdef CAIRO_RISCOS_FONTFACE_CACHE

#ifdef DEBUG_FONTCACHE

static void
cache_entry_stats (cairo_riscos_font_face_t *entry,
		   void *closure)
{
    printf ("    %s (ref count = %d)\n",
	    entry->name,
	    CAIRO_REFERENCE_COUNT_GET_VALUE (&entry->base.ref_count));
}

static void
fontcache_stats (cairo_fontface_cache *cache)
{
    printf ("Font Cache\n");
    printf ("----------\n");
    if (cache->used_face_count == 0)
	printf ("  Hash table is empty, there are no fonts currently in use.\n");
    else {
	printf ("  Fonts still in use (should be empty on exit):\n");
	_cairo_hash_table_foreach (cache->used_face_ht,
				   (cairo_hash_callback_func_t)cache_entry_stats,
				   NULL);
    }

    if (cache->unused_face_count == 0)
	printf ("  List is empty, there are no recently used fonts.\n");
    else {
	cairo_riscos_font_face_t *face;

	printf ("  Fonts recently used (ref counts should all be 1).\n");
	cairo_list_foreach_entry (face,
				  cairo_riscos_font_face_t,
				  &cache->unused_face_list,
				  link)
	{
	    cache_entry_stats (face, NULL);
	}    
    }
}

#endif

static void
cache_entry_destroy (cairo_riscos_font_face_t *entry,
		     void *closure)
{
    int count = CAIRO_REFERENCE_COUNT_GET_VALUE (&entry->base.ref_count);

    while (count--)
	cairo_font_face_destroy (&entry->base);
}

static void
cairo_riscos_free_fontface_cache (void)
{
    if (fontface_cache) {
	cairo_fontface_cache *cache = fontface_cache;

#ifdef DEBUG_FONTCACHE
	fontcache_stats (cache);
#endif

	if (cache->used_face_ht) {
	    _cairo_hash_table_foreach (cache->used_face_ht,
				       (cairo_hash_callback_func_t)cache_entry_destroy,
				       NULL);
	    _cairo_hash_table_destroy (cache->used_face_ht);
	}

	if (!cairo_list_is_empty (&cache->unused_face_list)) {
	    cairo_riscos_font_face_t *face, *next;

	    cairo_list_foreach_entry_safe (face,
					   next,
					   cairo_riscos_font_face_t,
					   &cache->unused_face_list,
					   link)
	    {
		cairo_list_init (&face->link);
		cairo_font_face_destroy (&face->base);
	    }
	}

	free (cache);

	fontface_cache = NULL;
    }
}

static cairo_status_t
cairo_riscos_create_fontface_cache (void)
{
    fontface_cache = malloc (sizeof (*fontface_cache));
    if (fontface_cache == NULL)
	return CAIRO_STATUS_NO_MEMORY;

    fontface_cache->used_face_ht = _cairo_hash_table_create (NULL);
    if (fontface_cache->used_face_ht == NULL) {
	free (fontface_cache);
	return CAIRO_STATUS_NO_MEMORY;
    }

    cairo_list_init (&fontface_cache->unused_face_list);
    fontface_cache->unused_face_count = 0;
    fontface_cache->used_face_count = 0;

    atexit (cairo_riscos_free_fontface_cache);

    return CAIRO_STATUS_SUCCESS;
}

#endif

static void
_cairo_riscos_font_face_destroy (void *abstract_face)
{
    cairo_riscos_font_face_t *rface = abstract_face;

#ifdef CAIRO_RISCOS_FONTFACE_CACHE
    cairo_fontface_cache *cache = fontface_cache;

    if (_cairo_hash_table_lookup (cache->used_face_ht,
				  &rface->base.hash_entry) == NULL)
    {
	/* Font face is not in the hash table of used faces, check if it's
	 * in the list of unused faces.  */
	if (cairo_list_is_empty (&rface->link)) {
#ifdef DEBUG_FONTCACHE
	    printf ("[%s]: Free resources used by font face '%s' (%p)\n",
		    __func__, rface->name, rface);
#endif
	    /* Font face is not in the hash table or list, so it's resources
	     * can be freed.  */
	    if (rface->name)
		free (rface->name);

	    if (rface->glyph_hash_map) {
		_cairo_hash_table_foreach (rface->glyph_hash_map,
					  _cairo_riscos_font_face_remove_glyph,
					  rface);

		_cairo_hash_table_destroy (rface->glyph_hash_map);
	    }

	    return;
	}
    }

    /* The font face is in the hash table, move to the usused list in case
     * we require it again soon. At this point the ref count is 0, we
     * need to increase it to 1 so that when we return to cairo_font_face_destroy,
     * it doesn't free its memory. */
    _cairo_hash_table_remove (cache->used_face_ht,
			      &rface->base.hash_entry);
    cache->used_face_count--;

    cairo_list_add (&rface->link, &cache->unused_face_list);
    cache->unused_face_count++;

    _cairo_reference_count_inc (&rface->base.ref_count);

#ifdef DEBUG_FONTCACHE
    printf ("[%s]: Font face '%s' moved from hash table to list\n",
	    __func__, rface->name);
#endif

    /* Check if the unused face list is getting too big. If so, delete the oldest
     * entry.  */
    if (cache->unused_face_count > MAX_UNUSED_FACE_ENTRIES) {
	cairo_riscos_font_face_t *oldest;

#ifdef DEBUG_FONTCACHE
	printf ("[%s]: Font cache exceeded maximum length of %d; reducing by 1.\n",
		__func__, MAX_UNUSED_FACE_ENTRIES);
#endif

	oldest = cairo_list_last_entry (&fontface_cache->unused_face_list,
					cairo_riscos_font_face_t,
					link);
	assert (oldest);

	cairo_list_del (&oldest->link);
	cache->unused_face_count--;

	/* Clear the next/prev members so that when we recurse through this function,
	 * we actually free the entry.  */
	cairo_list_init (&oldest->link);

	cairo_font_face_destroy (&oldest->base);
	return;
    }
#else
    if (rface->name)
	free (rface->name);

    if (rface->glyph_hash_map) {
	_cairo_hash_table_foreach (rface->glyph_hash_map,
				   _cairo_riscos_font_face_remove_glyph,
				   rface);

	_cairo_hash_table_destroy (rface->glyph_hash_map);
    }
#endif
}

cairo_public cairo_font_face_t * INLINE
cairo_riscos_font_face_create (const char *name)
{
    cairo_riscos_font_face_t *font_face = NULL;
    cairo_status_t status;
#ifdef CAIRO_RISCOS_FONTFACE_CACHE
    cairo_hash_entry_t key;
    int name_len;
    cairo_fontface_cache *cache;

    /* Initialise on first use (must use the global variable here).  */
    if (fontface_cache == NULL) {
	status = cairo_riscos_create_fontface_cache ();
	if (status)
	    goto NO_MEMORY_FREE_ALL;
    }

    cache = fontface_cache;

    /* First check if the required font face is already in use.  */
    key.hash = _cairo_hash_string (name);
    font_face = _cairo_hash_table_lookup (cache->used_face_ht,
					  &key);
    if (font_face != NULL) {
#ifdef DEBUG_FONTCACHE
	printf (">> [%s]: Cache hit from used faces hashmap; '%s' already in use.\n", __func__, name);
#endif
	return cairo_font_face_reference (&font_face->base);
//	return &font_face->base;
    }

    name_len = strlen (name);
    /* The required font face is not already in use, check if it's been
     * used recently.  */
    cairo_list_foreach_entry (font_face, cairo_riscos_font_face_t,
			      &cache->unused_face_list, link)
    {
	if (strncmp (name, font_face->name, name_len) == 0) {
	    /* Found the font face in the unused face list, move to the used face
	     * hash table.  */
#ifdef DEBUG_FONTCACHE
	    printf (">> [%s]: Cache hit from unused faces list; '%s' has been used recently.\n",
		    __func__, name);
#endif
	    cairo_list_del (&font_face->link);
	    cache->unused_face_count--;
	    
	    font_face->base.hash_entry.hash = key.hash;
	    status = _cairo_hash_table_insert (cache->used_face_ht,
					       &font_face->base.hash_entry);
	    cache->used_face_count++;

	    if (unlikely (status))
		goto NO_MEMORY;

	    /* Don't use cairo_font_face_reference() - we should already have a ref count
	     * of 1 from _cairo_riscos_font_face_destroy().  */
	    return &font_face->base;
	}
    }

#ifdef DEBUG_FONTCACHE
    printf ("[%s]: Cache miss; Loading font '%s'.\n",
	    __func__, name);
#endif

    /* The required font face has not been used recently, so load it now.  */
#endif

    font_face = malloc (sizeof (cairo_riscos_font_face_t));
    if (unlikely (font_face == NULL))
	goto NO_MEMORY;

    font_face->name = malloc (strlen (name) + 1);
    if (unlikely (font_face->name == NULL))
	goto NO_MEMORY_FREE_ALL;

    strcpy (font_face->name, name);

    font_face->glyph_hash_map = _cairo_hash_table_create (_cairo_riscos_glyphs_equal);
    if (unlikely (font_face->glyph_hash_map == NULL))
	goto NO_MEMORY_FREE_ALL;

    status = _cairo_riscos_read_font (font_face, name);

    _cairo_font_face_init (&font_face->base, &_cairo_riscos_font_face_backend);

#ifdef CAIRO_RISCOS_FONTFACE_CACHE
    if (likely (status) == CAIRO_STATUS_SUCCESS) {
	font_face->base.hash_entry.hash = key.hash;
	status = _cairo_hash_table_insert (cache->used_face_ht,
					   &font_face->base.hash_entry);
	cache->used_face_count++;
    }
#endif

    if (unlikely (status)) {
	status = _cairo_font_face_set_error (&font_face->base, status);
    }

    return &font_face->base;

NO_MEMORY_FREE_ALL:
    if (font_face != NULL) {
	if (font_face->name != NULL)
	    free (font_face->name);
	free (font_face);
    }

NO_MEMORY:
#ifdef DEBUG_FONTCACHE
    printf ("[%s]: Returned error\n", __func__);
#endif
    _cairo_error_throw (CAIRO_STATUS_NO_MEMORY);
    return (cairo_font_face_t *) &_cairo_font_face_nil;
}

const cairo_font_face_backend_t _cairo_riscos_font_face_backend = {
    CAIRO_FONT_TYPE_RISCOS,
    _cairo_riscos_font_face_create_for_toy,
    _cairo_riscos_font_face_destroy,
    _cairo_riscos_font_face_scaled_font_create
};

const char *
cairo_riscos_font_face_name (cairo_font_face_t *cface)
{
  cairo_riscos_font_face_t *rface = (cairo_riscos_font_face_t *)cface;

  if (cairo_font_face_get_type (cface) != CAIRO_FONT_TYPE_RISCOS)
    return NULL;

  return &rface->name[0];
}
