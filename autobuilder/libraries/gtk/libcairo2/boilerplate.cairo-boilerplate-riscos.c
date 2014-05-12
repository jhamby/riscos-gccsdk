/*
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Red Hat, Inc. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission. Red Hat, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * RED HAT, INC. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL RED HAT, INC. BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "cairo-boilerplate-private.h"

#include <cairo-riscos.h>

static cairo_surface_t *
_cairo_boilerplate_riscos_create_surface (const char		    *name,
					  cairo_content_t	     content,
					  double		     width,
					  double		     height,
					  double		     max_width,
					  double		     max_height,
					  cairo_boilerplate_mode_t   mode,
					  void			   **closure)
{
    cairo_format_t format;

    format = cairo_boilerplate_format_from_content (content);

    *closure = NULL;

    return cairo_riscos_sprite_surface_create (format, width, height);
}

static const cairo_boilerplate_target_t targets[] = {
    {
	"riscos", "riscos", NULL, NULL,
	CAIRO_SURFACE_TYPE_RISCOS_SPRITE, CAIRO_CONTENT_COLOR_ALPHA, 0,
	"cairo_riscos_sprite_surface_create",
	_cairo_boilerplate_riscos_create_surface,
	cairo_surface_create_similar,
	NULL, NULL,
	_cairo_boilerplate_get_image_surface,
	cairo_surface_write_to_png,
	NULL, NULL, NULL,
	TRUE, FALSE, FALSE
    },
    {
	"riscos", "riscos", NULL, NULL,
	CAIRO_SURFACE_TYPE_RISCOS_SPRITE, CAIRO_CONTENT_COLOR, 0,
	"cairo_quartz_surface_create",
	_cairo_boilerplate_riscos_create_surface,
	cairo_surface_create_similar,
	NULL, NULL,
	_cairo_boilerplate_get_image_surface,
	cairo_surface_write_to_png,
	NULL, NULL, NULL,
        FALSE, FALSE, FALSE
    },
};
CAIRO_BOILERPLATE (riscos, targets)
