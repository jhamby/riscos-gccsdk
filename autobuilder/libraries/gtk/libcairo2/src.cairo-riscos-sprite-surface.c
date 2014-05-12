/* cairo - a vector graphics library with display and print output
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
 * http://www.mozilla.og/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * Contributor(s):
 * 		Lee Noar.
 */

#include "cairo-riscos.h"
#include "cairo-surface-private.h"
#include "cairo-surface-fallback-private.h"
#include "cairo-image-surface-private.h"
#include "cairo-default-context-private.h"
#include "cairoint.h"

#include "oslib/osspriteop.h"

#include <kernel.h>
#include <swis.h>

static const cairo_surface_backend_t _cairo_riscos_sprite_surface_backend;
static const os_error *_cairo_last_oserror = NULL;

/**
 * SECTION:cairo-riscos
 * @Title: RISC OS Sprite Surfaces
 * @Short_DESCRIPTION: Rendering to sprites
 * @See_Also: #cairo_surface_t
 * 
 * RISC OS sprite surfaces are based on the Image surface, so anything
 * that can be rendered to an Image Surface can be rendered to a Sprite surface.
 **/

/**
 * CAIRO_HAS_RISCOS_SPRITE_SURFACE:
 * 
 * Defined if the RISC OS Sprite surface backend is available.
 **/

struct _cairo_riscos_sprite_surface {
    /* The sprite surface is derived from an image surface.  */
    cairo_image_surface_t	image;

    /* The address of the sprite area used for this surface.  */
    osspriteop_area 		*sprite_area;
    /* The size of the sprite area in bytes.  */
    int				sprite_area_size;
    /* Unique id of this surface.  */
    int				ID;
};

static cairo_bool_t
_cairo_surface_is_riscos (cairo_surface_t *surface)
{
  /* We can't test the surface type because we're claiming to be an Image.  */
  return surface->backend == &_cairo_riscos_sprite_surface_backend;
}

/**
 * cairo_riscos_sprite_surface_write_to_file:
 * @abstract_surface: the Cairo RISC OS sprite surface
 * @filename: The name of the file to write the sprite to.
 * 
 * Save the surface to to file as a RISC OS sprite file.
 * 
 * Return value: %CAIRO_STATUS_SUCCESS or %CAIRO_STATUS_RISCOS_ERROR if the sprite
 * could not be saved. %CAIRO_STATUS_SURFACE_TYPE_MISMATCH if the wrong type of
 * surface is passed in.
 */
cairo_status_t
cairo_riscos_sprite_surface_write_to_file (cairo_surface_t *abstract_surface,
					   const char *filename)
{
    os_error *err;

    cairo_riscos_sprite_surface_t *surface = (cairo_riscos_sprite_surface_t *)abstract_surface;
    
    if (!_cairo_surface_is_riscos (abstract_surface))
	return _cairo_surface_set_error (abstract_surface,
					 _cairo_error (CAIRO_STATUS_SURFACE_TYPE_MISMATCH));

    err = xosspriteop_save_sprite_file (osspriteop_PTR,
					surface->sprite_area,
					filename);
    return cairo_riscos_error ((_kernel_oserror *)err);
}

cairo_status_t
cairo_riscos_switch_output_to_surface (cairo_surface_t *abstract_surface)
{
    const cairo_riscos_sprite_surface_t *surface;
    os_error *err;
    char name[16];

    surface = (cairo_riscos_sprite_surface_t *)abstract_surface;

    sprintf (name, "surface%d", surface->ID);

    err = xosspriteop_switch_output_to_sprite (osspriteop_NAME,
					       surface->sprite_area,
					       (osspriteop_id)name,
					       NULL,
					       NULL,
					       NULL,
					       NULL,
					       NULL);
    return cairo_riscos_error ((_kernel_oserror *)err);
}

cairo_status_t
cairo_riscos_switch_output_from_surface (cairo_surface_t *abstract_surface)
{
    const cairo_riscos_sprite_surface_t *surface;
    os_error *err;

    surface = (cairo_riscos_sprite_surface_t *)abstract_surface;

    err = xosspriteop_switch_output_to_sprite (osspriteop_NAME,
					       surface->sprite_area,
					       0,
					       NULL,
					       NULL,
					       NULL,
					       NULL,
					       NULL); 
    return cairo_riscos_error ((_kernel_oserror *)err);
}

static os_mode
_cairo_riscos_sprite_type (cairo_format_t format)
{
    /* Using new style sprite mode word.  */
    unsigned int type = 1;
    
    switch (format)
    {
    case CAIRO_FORMAT_A1:
	type |= (1 << 27);		/* 1bpp */
	break;
    case CAIRO_FORMAT_A8:
	type |= (4 << 27);		/* 8bpp */
	break;
    case CAIRO_FORMAT_RGB16_565:
	type |= (5 << 27);		/* 16bpp */
	break;
    case CAIRO_FORMAT_RGB24:
    case CAIRO_FORMAT_RGB30:
    case CAIRO_FORMAT_ARGB32:
    case CAIRO_FORMAT_INVALID:
    default:
	type |= (6 << 27);		/* 32 bpp */
	break;
    }

    /* Horizontal DPI.  */
    type |= (90 << 1);
    /* Vertical DPI.  */
    type |= (90 << 14);

    return (os_mode)type;
}

static const _kernel_oserror *
_cairo_riscos_sprite_surface_create_sprite (const char *name,
					    int width,
					    int height,
					    cairo_format_t format,
					    osspriteop_area **sprite_area_ret,
					    int *sprite_area_size_ret)
{
    const os_error *err = NULL;
    osspriteop_area *sprite_area = NULL;

    /* Start with the size of a sprite area header which is 4 words and the
     * size of a sprite header which is 11 words.
     * This works for 32 bpp and probably for 8 & 16 bpp, but not sure about 1bpp.  */
    int area_size = sizeof (osspriteop_area) + sizeof (osspriteop_header) +
			cairo_format_stride_for_width (format, width) * height;
			
    sprite_area = cairo_riscos_memory_alloc (area_size);
    if (sprite_area == NULL)
	goto error;

    sprite_area->size = area_size;
    sprite_area->first = 16;
    if ((err = xosspriteop_clear_sprites (osspriteop_USER_AREA, sprite_area)) != NULL)
	goto error;

    if ((err = xosspriteop_create_sprite (osspriteop_USER_AREA,
					  sprite_area,
					  name,
					  FALSE,
					  width,
					  height,
					  _cairo_riscos_sprite_type (format))) != NULL)
	goto error;

    *sprite_area_ret = sprite_area;
    *sprite_area_size_ret = area_size;

    return NULL;
error:
    if (sprite_area != NULL)
	cairo_riscos_memory_free (sprite_area, area_size);

    *sprite_area_ret = NULL;
    *sprite_area_size_ret = 0;

    if (err)
	_cairo_last_oserror = err;

    return (_kernel_oserror *)err;
}

static void *
_cairo_riscos_sprite_get_image_data (osspriteop_area *sprite_area)
{
    osspriteop_header *sprite;

    sprite = (osspriteop_header *)((unsigned)sprite_area + sprite_area->first);

    return (void*)((unsigned)sprite + sprite->image);
}

/**
  * Wrapper for _cairo_image_surface_finish that allows us to free the
  * additional memory used for the sprite area.
  **/
static cairo_status_t
_cairo_riscos_sprite_surface_finish (void *abstract_surface)
{
    cairo_riscos_sprite_surface_t *surface = abstract_surface;

    cairo_riscos_memory_free (surface->sprite_area, surface->sprite_area_size);
    
    return _cairo_image_surface_finish (abstract_surface);
}

static const cairo_surface_backend_t _cairo_riscos_sprite_surface_backend = {
    /* We claim to be an image so that we are compatible.  */
    CAIRO_SURFACE_TYPE_IMAGE,
    _cairo_riscos_sprite_surface_finish, /* finish */

    _cairo_default_context_create, /* only accessed through the surface functions */

    NULL, /* create_similar */
    NULL, /* create similar image */
    _cairo_image_surface_map_to_image,
    _cairo_image_surface_unmap_image,

    _cairo_image_surface_source,
    _cairo_image_surface_acquire_source_image,
    _cairo_image_surface_release_source_image,
    _cairo_image_surface_snapshot,

    NULL, /* copy_page */
    NULL, /* show_page */

    _cairo_image_surface_get_extents, /* get_extents */
    _cairo_image_surface_get_font_options,

    NULL, /* flush */
    NULL, /* mark_dirty_rectangle */

    _cairo_image_surface_paint,	    /* paint */
    _cairo_image_surface_mask,	    /* mask */
    _cairo_image_surface_stroke,    /* stroke */
    _cairo_image_surface_fill,	    /* fill */
    NULL, /* fill_stroke */
    _cairo_image_surface_glyphs,    /* show_glyphs */
    NULL, /* has_show_text_glyphs */
    NULL  /* show_text_glyphs */
};

static cairo_surface_t *
_cairo_riscos_sprite_surface_create_for_pixman_image (pixman_image_t		*pixman_image,
						      pixman_format_code_t	 pixman_format)
{
    cairo_riscos_sprite_surface_t *surface;

    surface = malloc (sizeof (cairo_riscos_sprite_surface_t));
    if (unlikely (surface == NULL))
	return _cairo_surface_create_in_error (_cairo_error (CAIRO_STATUS_NO_MEMORY));

    _cairo_surface_init (&surface->image.base,
			 &_cairo_riscos_sprite_surface_backend,
			 NULL, /* device */
			 _cairo_content_from_pixman_format (pixman_format));

    _cairo_image_surface_init (&surface->image, pixman_image, pixman_format);

    return &surface->image.base;
}

static cairo_surface_t *
_cairo_riscos_sprite_surface_create_with_pixman_format (unsigned char		*data,
							pixman_format_code_t	 pixman_format,
							int			 width,
							int			 height,
							int			 stride)
{
    cairo_surface_t *surface;
    pixman_image_t *pixman_image;

    pixman_image = pixman_image_create_bits (pixman_format, width, height,
					     (uint32_t *) data, stride);

    if (unlikely (pixman_image == NULL))
	return _cairo_surface_create_in_error (_cairo_error (CAIRO_STATUS_NO_MEMORY));

    surface = _cairo_riscos_sprite_surface_create_for_pixman_image (pixman_image,
								    pixman_format);
    if (unlikely (surface->status)) {
	pixman_image_unref (pixman_image);
	return surface;
    }

    /* we can not make any assumptions about the initial state of user data */
    surface->is_clear = data == NULL;
    return surface;
}

/**
  * Taken from cairo-image-surface.c and adapted for RISC OS.
  * For 24/32 bits - swapped the red and blue components.
  * For 16 bits - swapped the red and blue components and
  * return 5:5:5 instead of 5:6:5.
  * For 30 bits - not supported by RISC OS treat as 24 bit.
  **/
static pixman_format_code_t
_riscos_cairo_format_to_pixman_format_code (cairo_format_t format)
{
    pixman_format_code_t ret;
    switch (format) {
    case CAIRO_FORMAT_A1:
	ret = PIXMAN_a1;
	break;
    case CAIRO_FORMAT_A8:
	ret = PIXMAN_a8;
	break;
    case CAIRO_FORMAT_RGB24:
    case CAIRO_FORMAT_RGB30:
//	ret = PIXMAN_x8r8g8b8;
	ret = PIXMAN_x8b8g8r8;
	break;
    case CAIRO_FORMAT_RGB16_565:
	ret = PIXMAN_x1b5g5r5;
	break;
    case CAIRO_FORMAT_ARGB32:
    case CAIRO_FORMAT_INVALID:
    default:
	ret = PIXMAN_a8b8g8r8;
	break;
    }
    return ret;
}

/**
 * cairo_riscos_sprite_surface_create:
 * @format: format of pixels in the surface to create
 * @width: width of the surface, in pixels
 * @height: height of the surface, in pixels
 *
 * Creates a RISC OS sprite surface of the specified format and
 * dimensions.
 *
 * Return value: a pointer to the newly created surface. The caller
 * owns the surface and should call cairo_surface_destroy() when done
 * with it.
 *
 * This function always returns a valid pointer, but it will return a
 * pointer to a "nil" surface if an error such as out of memory
 * occurs. You can use cairo_surface_status() to check for this.
 **/
cairo_surface_t *
cairo_riscos_sprite_surface_create (cairo_format_t format,
				    int width,
				    int height)
{
    static int ID = 1;
    cairo_status_t status;
    cairo_riscos_sprite_surface_t *surface;
    osspriteop_area *sprite_area;
    void *sprite_data;
    int sprite_area_size;
    char name[16];

    if (width <= 0 || height <= 0)
	return _cairo_surface_create_in_error (_cairo_error (CAIRO_STATUS_INVALID_SIZE));

    /* Not sure how useful a name like this is yet. Doesn't really need to be unique,
     * because we're using a different sprite area for each sprite. Might be useful for
     * debugging.  */
    sprintf (name, "surface%d", ID);

    status = cairo_riscos_error (_cairo_riscos_sprite_surface_create_sprite (name,
									     width,
									     height,
									     format,
									     &sprite_area,
									     &sprite_area_size));
    if (unlikely (status))
	return _cairo_surface_create_in_error (_cairo_error (status));
    if (unlikely (sprite_area == NULL))
	return _cairo_surface_create_in_error (_cairo_error (CAIRO_STATUS_NO_MEMORY));

    sprite_data = _cairo_riscos_sprite_get_image_data (sprite_area);

    /* Set sprite data to all white.  */
//    memset (sprite_data, 0xff, cairo_format_stride_for_width (format, width) * height);

    surface = (cairo_riscos_sprite_surface_t *)_cairo_riscos_sprite_surface_create_with_pixman_format (
						sprite_data,
						_riscos_cairo_format_to_pixman_format_code (format),
						width,
						height,
						cairo_format_stride_for_width (format, width));
    if (unlikely (surface == NULL))
	return _cairo_surface_create_in_error (_cairo_error (CAIRO_STATUS_NO_MEMORY));

    surface->ID = ID++;
    surface->sprite_area = sprite_area;
    surface->sprite_area_size = sprite_area_size;

    return (cairo_surface_t *)surface;
}

/**
 * cairo_riscos_sprite_surface_get_info:
 * @abstract_surface: the Cairo RISC OS sprite surface
 * 
 * Return in @sprite_area_ret a pointer to the RISC OS sprite area used to implement
 * this surface and a pointer to the sprite in @sprite_pointer_ret.
 * 
 * Return value: %CAIRO_STATUS_SUCCESS or %CAIRO_STATUS_RISCOS_ERROR if the sprite
 * area info could not be determined.
 *
 **/
cairo_status_t
cairo_riscos_sprite_surface_get_info (cairo_surface_t *abstract_surface,
				      void **sprite_area_ret,
				      void **sprite_pointer_ret)
{
    const cairo_riscos_sprite_surface_t *surface = (cairo_riscos_sprite_surface_t *)abstract_surface;
    cairo_status_t status = abstract_surface->status;
    osspriteop_header *sprite_pointer = NULL;
    osspriteop_area *sprite_area = NULL;

    if (!_cairo_surface_is_riscos (abstract_surface))
	status = _cairo_surface_set_error (abstract_surface,
					   _cairo_error (CAIRO_STATUS_SURFACE_TYPE_MISMATCH));

    if (likely (status == CAIRO_STATUS_SUCCESS)) {
	char name[16];

	sprintf (name, "surface%d", surface->ID);

	status = cairo_riscos_error ((_kernel_oserror *)
				     xosspriteop_select_sprite (osspriteop_USER_AREA,
								surface->sprite_area,
								(osspriteop_id)name,
								&sprite_pointer));
	sprite_area = surface->sprite_area;
    }
    
    if (sprite_area_ret)
	*sprite_area_ret = sprite_area;
    if (sprite_pointer_ret)
	*sprite_pointer_ret = sprite_pointer;

    return status;
}

void
cairo_riscos_sprite_surface_set_size (cairo_surface_t *abstract_surface,
				      int width,
				      int height)
{
    cairo_status_t status;
    cairo_riscos_sprite_surface_t *surface = (cairo_riscos_sprite_surface_t *)abstract_surface;
    cairo_format_t format = cairo_image_surface_get_format (abstract_surface);
    unsigned int ref_count = CAIRO_REFERENCE_COUNT_GET_VALUE (&abstract_surface->ref_count);

    char name[16];
    sprintf (name, "surface%d", surface->ID);
    
    if (width <= 0 || height <= 0)
        return;

    if (!_cairo_surface_is_riscos (abstract_surface)) {
	_cairo_surface_set_error (abstract_surface,
				  _cairo_error (CAIRO_STATUS_SURFACE_TYPE_MISMATCH));
	return;
    }

    _cairo_riscos_sprite_surface_finish (abstract_surface);
    status = cairo_riscos_error (_cairo_riscos_sprite_surface_create_sprite (name,
									     width,
									     height,
									     format,
									     &surface->sprite_area,
									     &surface->sprite_area_size));
    if (likely (status == CAIRO_STATUS_SUCCESS)) {
	pixman_image_t *pixman_image;
	void *sprite_data;
	pixman_format_code_t pixman_format;

	pixman_format = _riscos_cairo_format_to_pixman_format_code (format);
	sprite_data = _cairo_riscos_sprite_get_image_data (surface->sprite_area);
	pixman_image = pixman_image_create_bits (pixman_format,
						 width, height,
						 (uint32_t *) sprite_data,
						 cairo_format_stride_for_width (format, width));

	_cairo_surface_init (&surface->image.base,
			     &_cairo_riscos_sprite_surface_backend,
			     NULL, /* device */
			     _cairo_content_from_pixman_format (pixman_format));
	_cairo_image_surface_init (&surface->image, pixman_image, pixman_format);
	CAIRO_REFERENCE_COUNT_INIT (&abstract_surface->ref_count, ref_count);
    }

    _cairo_surface_set_error (abstract_surface, status);
}

const _kernel_oserror *
cairo_riscos_last_oserror (cairo_bool_t reset)
{
    const _kernel_oserror *err = (_kernel_oserror *)_cairo_last_oserror;

    if (reset)
	_cairo_last_oserror = NULL;

    return err;
}

cairo_status_t
cairo_riscos_error (const _kernel_oserror *err)
{
    if (err) {
	_cairo_last_oserror = (os_error *)err;
	return CAIRO_STATUS_RISCOS_ERROR;
    }
    else
	return CAIRO_STATUS_SUCCESS;
}
