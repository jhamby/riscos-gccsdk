/* Copyright (C) 1992, 1996, 1997, 1998 Aladdin Enterprises.  All rights reserved.

  This software is provided AS-IS with no warranty, either express or
  implied.

  This software is distributed under license and may not be copied,
  modified or distributed except as expressly authorized under the terms
  of the license contained in the file LICENSE in this distribution.
  
  For more information about licensing, please refer to
  http://www.ghostscript.com/licensing/. For information on
  commercial licensing, go to http://www.artifex.com/licensing/ or
  contact Artifex Software, Inc., 101 Lucas Valley Road #110,
  San Rafael, CA  94903, U.S.A., +1(415)492-9861.
 */

/*$Id: gdevspr1.c $ */
/* RISC OS Sprite device */
#include "gdevprn.h"
#include "gscdefs.h"
#include "gxlum.h"

/*
 * Thanks are due to Jos Vos (jos@bull.nl) for an earlier P*M driver,
 * on which this one is based.
 */

/*
 * There are 6 (pairs of) drivers here, plus one less related one:
 *      spr1[raw] - outputs spr1 (black and white).
 *      spr8m[raw] - outputs spr8m (gray-scale).
 *      sprm[raw] - outputs spr1 if the page contains only black and white,
 *        otherwise spr8m.
 *      spr8c[raw] - outputs spr8c (RGB).
 *      spr[raw] - outputs spr1 if the page contains only black and white,
 *        otherwise spr8m if the page contains only gray shades,
 *        otherwise spr8c.
 *      sprk[raw] - computes internally in CMYK, outputs spr8c (RGB).
 */

/*
 * The code here is designed to work with variable depths for spr8m and spr8c.
 * The code will work with any of the values in brackets, but the
 * Ghostscript imager requires that depth be a power of 2 or be 24,
 * so the actual allowed values are more limited.
 *      spr8m,sprm: 1, 2, 4, 8, 16.  [1-16]
 *      spr8mraw,sprmraw: 1, 2, 4, 8.  [1-8]
 *      spr8c,spr: 4(3x1), 8(3x2), 16(3x5), 24(3x8), 32(3x10).  [3-32]
 *      spr8craw,sprraw: 4(3x1), 8(3x2), 16(3x5), 24(3x8).  [3-24]
 *      sprk, sprkraw: 4(4x1), 8(4x2), 16(4x4), 32(4x8).  [4-32]
 */

/* All two-byte quantities are stored LSB-first! */
#if arch_is_big_endian
#  define assign_ushort(a,v) a = ((v) >> 8) + ((v) << 8)
#  define assign_ulong(a,v) a = ( ( (v) & 0xff ) << 24 ) + ( ( (v) & 0xff00 ) << 8 ) + ( ( (v) & 0xff0000 ) >> 8 ) + ( ( (v) & 0xff000000 ) >> 24 ) 
#else
#  define assign_ushort(a,v) a = (v)
#  define assign_ulong(a,v) a = (v)
#endif

typedef struct sprite_area_s {
      int sprite_count;
      int first;
      int used;
} sprite_area;

typedef struct sprite_header_s {
      int size;
      char name [12];
      int width;
      int height;
      int left_bit;
      int right_bit;
      int image;
      int mask;
      int mode;
} sprite_header;

/* Structure for P*M devices, which extend the generic printer device. */

#define MAX_COMMENT 70		/* max user-supplied comment */
struct gx_device_spr_s {
    gx_device_common;
    gx_prn_device_common;
    /* Additional state for P*M devices */
    char magic;			/* n for "Pn" */
    char comment[MAX_COMMENT + 1];	/* comment for head of file */
    byte is_raw;		/* 1 if raw format, 0 if plain */
    byte optimize;		/* 1 if optimization OK, 0 if not */
    byte uses_color;		/* 0 if image is black and white, */
    /* 1 if gray (spr8m or spr8c only), */
    /* 2 or 3 if colored (spr8c only) */
    int alpha_text;		/* # of alpha bits for text (1,2,4) */
    int alpha_graphics;		/* ditto for graphics (1,2,4) */
        dev_proc_copy_alpha((*save_copy_alpha));
};
typedef struct gx_device_spr_s gx_device_spr;

#define bdev ((gx_device_spr *)pdev)

/* ------ The device descriptors ------ */

/*
 * Default X and Y resolution.
 */
#define X_DPI 72
#define Y_DPI 72

/* Macro for generating P*M device descriptors. */
#define spr1_prn_device(procs, dev_name, magic, is_raw, num_comp, depth, max_gray, max_rgb, optimize, x_dpi, y_dpi, print_page)\
{	prn_device_body(gx_device_spr, procs, dev_name,\
	  DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS, x_dpi, y_dpi,\
	  0, 0, 0, 0,\
	  num_comp, depth, max_gray, max_rgb, max_gray + 1, max_rgb + 1,\
	  print_page),\
	magic,\
	 { 0 },\
	is_raw,\
	optimize,\
	0, 1, 1, 0\
}

/* For all but spr1, we need our own color mapping and alpha procedures. */
static dev_proc_map_rgb_color(spr8m_map_rgb_color);
static dev_proc_map_rgb_color(spr8c_map_rgb_color);
static dev_proc_map_color_rgb(spr8m_map_color_rgb);
static dev_proc_map_color_rgb(spr8c_map_color_rgb);
static dev_proc_map_cmyk_color(sprk_map_cmyk_color);
static dev_proc_map_color_rgb(sprk_map_color_rgb);
static dev_proc_put_params(spr8c_put_params);
static dev_proc_get_alpha_bits(spr8c_get_alpha_bits);
static dev_proc_copy_alpha(spr_copy_alpha);

/* We need to initialize uses_color when opening the device, */
/* and after each showpage. */
static dev_proc_open_device(spr_open);
static dev_proc_output_page(spr_output_page);

/* And of course we need our own print-page routines. */
static dev_proc_print_page(spr1_print_page);
static dev_proc_print_page(spr8m_print_page);
static dev_proc_print_page(spr8c_print_page);
static dev_proc_print_page(sprk_print_page);

/* The device procedures */

static const gx_device_procs spr1_procs =
prn_procs(gdev_prn_open, spr_output_page, gdev_prn_close);

/* See gdevprn.h for the template for the following. */
#define pgpm_procs(p_map_rgb_color, p_map_color_rgb, p_map_cmyk_color) {\
	spr_open, NULL, NULL, spr_output_page, gdev_prn_close,\
	p_map_rgb_color, p_map_color_rgb, NULL, NULL, NULL, NULL, NULL, NULL,\
	gdev_prn_get_params, spr8c_put_params,\
	p_map_cmyk_color, NULL, NULL, NULL, gx_page_device_get_page_device,\
	spr8c_get_alpha_bits\
}

static const gx_device_procs spr8m_procs =
pgpm_procs(spr8m_map_rgb_color, spr8m_map_color_rgb, NULL);
static const gx_device_procs spr8c_procs =
pgpm_procs(spr8c_map_rgb_color, spr8c_map_color_rgb, NULL);
static const gx_device_procs sprk_procs =
pgpm_procs(NULL, sprk_map_color_rgb, sprk_map_cmyk_color);

/* The device descriptors themselves */
const gx_device_spr gs_spr1_device =
spr1_prn_device(spr1_procs, "spr1", '1', 0, 1, 1, 1, 0, 0,
	       X_DPI, Y_DPI, spr1_print_page);
const gx_device_spr gs_spr1raw_device =
spr1_prn_device(spr1_procs, "spr1raw", '4', 1, 1, 1, 1, 1, 0,
	       X_DPI, Y_DPI, spr1_print_page);
const gx_device_spr gs_spr8m_device =
spr1_prn_device(spr8m_procs, "spr8m", '2', 0, 1, 8, 255, 0, 0,
	       X_DPI, Y_DPI, spr8m_print_page);
const gx_device_spr gs_spr8mraw_device =
spr1_prn_device(spr8m_procs, "spr8mraw", '5', 1, 1, 8, 255, 0, 0,
	       X_DPI, Y_DPI, spr8m_print_page);
const gx_device_spr gs_sprm_device =
spr1_prn_device(spr8m_procs, "sprm", '2', 0, 1, 8, 255, 0, 1,
	       X_DPI, Y_DPI, spr8m_print_page);
const gx_device_spr gs_sprmraw_device =
spr1_prn_device(spr8m_procs, "sprmraw", '5', 1, 1, 8, 255, 0, 1,
	       X_DPI, Y_DPI, spr8m_print_page);
const gx_device_spr gs_spr8c_device =
spr1_prn_device(spr8c_procs, "spr8c", '3', 0, 3, 24, 255, 255, 0,
	       X_DPI, Y_DPI, spr8c_print_page);
const gx_device_spr gs_spr8craw_device =
spr1_prn_device(spr8c_procs, "spr8craw", '6', 1, 3, 24, 255, 255, 0,
	       X_DPI, Y_DPI, spr8c_print_page);
const gx_device_spr gs_spr_device =
spr1_prn_device(spr8c_procs, "spr", '3', 0, 3, 24, 255, 255, 1,
	       X_DPI, Y_DPI, spr8c_print_page);
const gx_device_spr gs_sprraw_device =
spr1_prn_device(spr8c_procs, "sprraw", '6', 1, 3, 24, 255, 255, 1,
	       X_DPI, Y_DPI, spr8c_print_page);
const gx_device_spr gs_sprk_device =
spr1_prn_device(sprk_procs, "sprk", '3', 0, 4, 4, 1, 1, 0,
	       X_DPI, Y_DPI, sprk_print_page);
const gx_device_spr gs_sprkraw_device =
spr1_prn_device(sprk_procs, "sprkraw", '6', 1, 4, 4, 1, 1, 0,
	       X_DPI, Y_DPI, sprk_print_page);

/* ------ Initialization ------ */

/* Set the copy_alpha procedure if necessary. */
static void
spr8c_set_copy_alpha(gx_device * pdev)
{
    if (dev_proc(pdev, copy_alpha) != spr_copy_alpha) {
	bdev->save_copy_alpha = dev_proc(pdev, copy_alpha);
	if (pdev->color_info.depth > 4)
	    set_dev_proc(pdev, copy_alpha, spr_copy_alpha);
    }
}

static int
spr_open(gx_device * pdev)
{
    int code = gdev_prn_open(pdev);

    if (code < 0)
	return code;
    bdev->uses_color = 0;
    spr8c_set_copy_alpha(pdev);
    return code;
}

/* Print a page, and reset uses_color if this is a showpage. */
static int
spr_output_page(gx_device * pdev, int num_copies, int flush)
{
    int code = gdev_prn_output_page(pdev, num_copies, flush);

    if (code < 0)
	return code;
    if (flush)
	bdev->uses_color = 0;
    return code;
}

/* ------ Color mapping routines ------ */

/* Map an RGB color to a spr8m gray value. */
/* Keep track of whether the image is black-and-white or gray. */
static gx_color_index
spr8m_map_rgb_color(gx_device * pdev, const gx_color_value cv[])
{				/* We round the value rather than truncating it. */
	gx_color_value r, g, b;
    r = cv[0]; g = cv[1]; b = cv[2];
    gx_color_value gray =
    ((r * (ulong) lum_red_weight) +
     (g * (ulong) lum_green_weight) +
     (b * (ulong) lum_blue_weight) +
     (lum_all_weights / 2)) / lum_all_weights
    * pdev->color_info.max_gray / gx_max_color_value;

    if (!(gray == 0 || gray == pdev->color_info.max_gray))
	bdev->uses_color = 1;
    return gray;
}

/* Map a spr8m gray value back to an RGB color. */
static int
spr8m_map_color_rgb(gx_device * dev, gx_color_index color, ushort prgb[3])
{
    gx_color_value gray =
    color * gx_max_color_value / dev->color_info.max_gray;

    prgb[0] = gray;
    prgb[1] = gray;
    prgb[2] = gray;
    return 0;
}

/* Map an RGB color to a spr8c color tuple. */
/* Keep track of whether the image is black-and-white, gray, or colored. */
static gx_color_index
spr8c_map_rgb_color(gx_device * pdev, const gx_color_value cv[])
{
    uint bitspercolor = pdev->color_info.depth / 3;
    ulong max_value = pdev->color_info.max_color;
	gx_color_value r, g, b;
	r = cv[0]; g = cv[1]; b = cv[2];
    gx_color_value rc = r * max_value / gx_max_color_value;
    gx_color_value gc = g * max_value / gx_max_color_value;
    gx_color_value bc = b * max_value / gx_max_color_value;

    if (rc == gc && gc == bc) {	/* black-and-white or gray */
	if (!(rc == 0 || rc == max_value))
	    bdev->uses_color |= 1;	/* gray */
    } else			/* color */
	bdev->uses_color = 2;
    return ((((ulong) bc << bitspercolor) + gc) << bitspercolor) + rc;
}

/* Map a spr8c color tuple back to an RGB color. */
static int
spr8c_map_color_rgb(gx_device * dev, gx_color_index color, ushort prgb[3])
{
    uint bitspercolor = dev->color_info.depth / 3;
    uint colormask = (1 << bitspercolor) - 1;
    uint max_rgb = dev->color_info.max_color;

    prgb[0] = ((color >> (bitspercolor * 2)) & colormask) *
	(ulong) gx_max_color_value / max_rgb;
    prgb[1] = ((color >> bitspercolor) & colormask) *
	(ulong) gx_max_color_value / max_rgb;
    prgb[2] = (color & colormask) *
	(ulong) gx_max_color_value / max_rgb;
    return 0;
}

/* Map a CMYK color to a pixel value. */
static gx_color_index
sprk_map_cmyk_color(gx_device * pdev, const gx_color_value cv[])
{
    uint bpc = pdev->color_info.depth >> 2;
	gx_color_value c, m, y, k;
	c = cv[0]; m = cv[1]; y = cv[2]; k = cv[3];
    if (bpc == 1) {		/* also know max_value == 1 */
	return
	    ((c == gx_max_color_value) << 3) +
	    ((m == gx_max_color_value) << 2) +
	    ((y == gx_max_color_value) << 1) +
	    (k == gx_max_color_value);
    } else {
	ulong max_value = pdev->color_info.max_color;
	uint cc = c * max_value / gx_max_color_value;
	uint mc = m * max_value / gx_max_color_value;
	uint yc = y * max_value / gx_max_color_value;
	uint kc = k * max_value / gx_max_color_value;
	gx_color_index color =
	((((((ulong) cc << bpc) + mc) << bpc) + yc) << bpc) + kc;

	return (color == gx_no_color_index ? color ^ 1 : color);
    }
}

/* Map a CMYK pixel value to RGB. */
static int
sprk_map_color_rgb(gx_device * dev, gx_color_index color, gx_color_value rgb[3])
{
    int bpc = dev->color_info.depth >> 2;

    if (bpc == 1) {
	/* Standard 4-bit CMYK */
	if (color & 1)
	    rgb[0] = rgb[1] = rgb[2] = 0;
	else {
	    rgb[0] = (color & 8 ? 0 : gx_max_color_value);
	    rgb[1] = (color & 4 ? 0 : gx_max_color_value);
	    rgb[2] = (color & 2 ? 0 : gx_max_color_value);
	}
    } else {
	gx_color_index cshift = color;
	uint mask = (1 << bpc) - 1;
	uint k = cshift & mask;
	uint y = (cshift >>= bpc) & mask;
	uint m = (cshift >>= bpc) & mask;
	uint c = cshift >> bpc;
	uint max_value = dev->color_info.max_color;
	uint not_k = max_value - k;

#define cvalue(c)\
    ((gx_color_value)((ulong)(c) * gx_max_color_value / max_value))
	/* We use our improved conversion rule.... */
	rgb[0] = cvalue((max_value - c) * not_k / max_value);
	rgb[1] = cvalue((max_value - m) * not_k / max_value);
	rgb[2] = cvalue((max_value - y) * not_k / max_value);
#undef cvalue
    }
    return 0;
}

/* ------ Alpha capability ------ */

/* Put parameters. */
static int
spr8c_put_alpha_param(gs_param_list * plist, gs_param_name param_name, int *pa,
		    bool alpha_ok)
{
    int code = param_read_int(plist, param_name, pa);

    switch (code) {
	case 0:
	    switch (*pa) {
		case 1:
		    return 0;
		case 2:
		case 4:
		    if (alpha_ok)
			return 0;
		default:
		    code = gs_error_rangecheck;
	    }
	default:
	    param_signal_error(plist, param_name, code);
	case 1:
	    ;
    }
    return code;
}
static int
spr8c_put_params(gx_device * pdev, gs_param_list * plist)
{
    gx_device_color_info save_info;
    int ncomps = pdev->color_info.num_components;
    int bpc = pdev->color_info.depth / ncomps;
    int ecode = 0;
    int code;
    int atext = bdev->alpha_text, agraphics = bdev->alpha_graphics;
    bool alpha_ok;
    long v;
    const char *vname;

    save_info = pdev->color_info;
    if ((code = param_read_long(plist, (vname = "GrayValues"), &v)) != 1 ||
	(code = param_read_long(plist, (vname = "RedValues"), &v)) != 1 ||
	(code = param_read_long(plist, (vname = "GreenValues"), &v)) != 1 ||
	(code = param_read_long(plist, (vname = "BlueValues"), &v)) != 1
	) {
	if (code < 0)
	    ecode = code;
	else if (v < 2 || v > (bdev->is_raw || ncomps > 1 ? 256 : 65536L))
	    param_signal_error(plist, vname,
			       ecode = gs_error_rangecheck);
	else if (v == 2)
	    bpc = 1;
	else if (v <= 4)
	    bpc = 2;
	else if (v <= 16)
	    bpc = 4;
	else if (v <= 32 && ncomps == 3)
	    bpc = 5;
	else if (v <= 256)
	    bpc = 8;
	else
	    bpc = 16;
	if (ecode >= 0) {
	    static const byte depths[4][16] =
	    {
		{1, 2, 0, 4, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 16},
		{0},
		{4, 8, 0, 16, 16, 0, 0, 24},
		{4, 8, 0, 16, 0, 0, 0, 32},
	    };

	    pdev->color_info.depth = depths[ncomps - 1][bpc - 1];
	    pdev->color_info.max_gray = pdev->color_info.max_color =
		(pdev->color_info.dither_grays =
		 pdev->color_info.dither_colors = (int)v) - 1;
	}
    }
    alpha_ok = bpc >= 5;
    if ((code = spr8c_put_alpha_param(plist, "TextAlphaBits", &bdev->alpha_text, alpha_ok)) < 0)
	ecode = code;
    if ((code = spr8c_put_alpha_param(plist, "GraphicsAlphaBits", &bdev->alpha_graphics, alpha_ok)) < 0)
	ecode = code;
    if ((code = ecode) < 0 ||
	(code = gdev_prn_put_params(pdev, plist)) < 0
	) {
	bdev->alpha_text = atext;
	bdev->alpha_graphics = agraphics;
	pdev->color_info = save_info;
    }
    spr8c_set_copy_alpha(pdev);
    return code;
}

/* Get the number of alpha bits. */
static int
spr8c_get_alpha_bits(gx_device * pdev, graphics_object_type type)
{
    return (type == go_text ? bdev->alpha_text : bdev->alpha_graphics);
}

/* Copy an alpha map, noting whether we may generate some non-black/white */
/* colors through blending. */
static int
spr_copy_alpha(gx_device * pdev, const byte * data, int data_x,
	   int raster, gx_bitmap_id id, int x, int y, int width, int height,
	       gx_color_index color, int depth)
{
    if (pdev->color_info.depth < 24 ||
	(color >> 8) == (color & 0xffff)
	)
	bdev->uses_color |= 1;
    else
	bdev->uses_color |= 2;
    return (*bdev->save_copy_alpha) (pdev, data, data_x, raster, id,
				     x, y, width, height, color, depth);
}

/* ------ Internal routines ------ */

/* Print a page using a given row printing routine. */
static int
spr_print_page_loop(gx_device_printer * pdev, char magic, FILE * pstream,
	     int (*row_proc) (gx_device_printer *, byte *, int, FILE *))
{
    uint raster = gdev_prn_raster(pdev);
    byte *data = (byte *) gs_alloc_bytes(pdev->memory, raster, "spr_begin_page");
    int lnum = 0;
    int code = 0;
    int spr_size, spr_width, spr_right_bit, spr_mode;
    sprite_area spr_area;
    sprite_header spr_header;

    if (data == 0)
	return_error(gs_error_VMerror);
    /* Hack.  This should be done in the callers.  */

    /* Need to output an appropriate header here ... perhaps knick some
    structures from somewhere like oslib */

    assign_ulong( spr_area.sprite_count, 1 );
    assign_ulong( spr_area.first, sizeof( sprite_area ) + sizeof( int ) );

    strncpy( spr_header.name, "gssprite", 12 );
    assign_ulong( spr_header.height, ( pdev->height - 1 ) );
    spr_header.left_bit = 0;
    assign_ulong( spr_header.image, sizeof( sprite_header ) );
    assign_ulong( spr_header.mask, sizeof( sprite_header ) );

/*    fprintf( stderr, "Colour depth = %d, max grey = %d\n",
             pdev->color_info.depth, pdev->color_info.max_gray ); */

    switch (magic) {
	case '1':		/* spr1 */
	case '4':		/* spr1raw */
/*	     fprintf( stderr, "1bpp mono\n" ); */
	 /* Size is width + max of 31 more bits shifted down by 3 bits = bytes */
         spr_size = ( ( pdev->width + 31 ) >> 5 ) * 4 * pdev->height + 8*2;
         spr_width = ( ( pdev->width + 31 ) >> 5 ) - 1; /* Divide by 32 */
         spr_right_bit = ( ( pdev->width % 32 ) == 0 ) ? 31 : ( ( pdev->width % 32 ) - 1 );
         spr_mode = 18;
         assign_ulong( spr_header.image, sizeof( sprite_header ) + 8*2 ); /* Override */
         assign_ulong( spr_header.mask, sizeof( sprite_header ) + 8*2 );
	     /* 1 bpp sprite */
	    break;

	case '2':		/* spr8m */
	case '5':		/* spr8mraw */
/*	     fprintf( stderr, "8bpp grey\n" ); */
	     /* Looks like I need a mode 15 sprite with a palette */
         spr_size = pdev->width;
         spr_size += ( ( pdev->width % 4 ) == 0 ) ? 0 : 4 - ( pdev->width % 4 );
         spr_size *= pdev->height;
         spr_size += 8*256; /* two words per 256 entries */
         spr_width = ( ( pdev->width + 3 ) >> 2 ) - 1; /* 4 bytes per word so shift 2 bits */
         spr_right_bit = ( ( pdev->width % 4 ) == 0 ) ? 31 : ( ( pdev->width % 4 )*8 - 1 );
         spr_mode = 21;
         assign_ulong( spr_header.image, sizeof( sprite_header ) + 8*256 ); /* Override */
         assign_ulong( spr_header.mask, sizeof( sprite_header ) + 8*256 );
	     /* 8 bpp mono sprite */
	    break;

	case '3':		/* spr8c */
	case '6':		/* spr8craw */
/*	     fprintf( stderr, "8bpp colour\n" ); */
         spr_size = pdev->width * 4 * pdev->height; /* 4 bytes per word, per line */
         spr_width = pdev->width - 1;
         spr_right_bit = 31;
         spr_mode = ( 6 << 27 ) + ( 90 << 14 ) + ( 90 << 1 ) + 1;
         /* Though may not be 6 'cos of 16bpp etc */
	     /* (max) 8 bpp colour sprite */
	    break;

	default:
		/* fprintf( stderr, "Output format not recognized\n" ); */
		break;
    }

    assign_ulong( spr_header.size, spr_size + sizeof( sprite_header ) );  
    assign_ulong( spr_header.width, spr_width );
    assign_ulong( spr_header.right_bit, spr_right_bit );
    assign_ulong( spr_header.mode, spr_mode );
    assign_ulong( spr_area.used, sizeof( sprite_area ) + sizeof( int ) + spr_size + sizeof( sprite_header ) );

    /* Now output the area and header */
    fwrite( ( void * )&spr_area, 1, sizeof( sprite_area ), pstream);
    fwrite( ( void * )&spr_header, 1, sizeof( sprite_header ), pstream);

    if( spr_mode < 256 ) {
        int col, i, start = 0, step = 0, end = 0;
        switch( magic ) {
            case '1':		/* spr1 */
            case '4':		/* spr1raw */
                start = 255; step = -255; end = -255; break;
            case '2':
            case '5':
                start = 0; step = 1; end = 256; break;
        }
        /* Output a palette */
        for( i = start; i != end ; i += step ) {
             assign_ulong( col, ( ( i << 24 ) + ( i << 16 ) + ( i << 8 ) + 0x10 ) );
             fwrite( &col, 1, sizeof( int ), pstream );
             fwrite( &col, 1, sizeof( int ), pstream );
        }
    }

    for (; lnum < pdev->height; lnum++) {
	byte *row;

	code = gdev_prn_get_bits(pdev, lnum, data, &row);
	if (code < 0)
	    break;
	code = (*row_proc) (pdev, row, pdev->color_info.depth, pstream);
	if (code < 0)
	    break;
    }
    gs_free_object(pdev->memory, data, "spr_print_page_loop");
    return (code < 0 ? code : 0);
}

/* ------ Individual page printing routines ------ */

/* Print a monobit page. */
static int
spr1_print_row(gx_device_printer * pdev, byte * data, int depth,
	      FILE * pstream)
{
/*    if (bdev->is_raw)
	fwrite(data, 1, (pdev->width + 7) >> 3, pstream);
    else */ {
	byte *bp, col;
	uint x, mask;

	for (bp = data, x = 0; x < ( ( pdev->width + 7 ) >> 3 ); x++) {
	    /* There's a better way to do this! */
	    col = ( ( *bp & 0x01 ) << 7 ) +
	          ( ( *bp & 0x02 ) << 5 ) +
	          ( ( *bp & 0x04 ) << 3 ) +
	          ( ( *bp & 0x08 ) << 1 ) +
	          ( ( *bp & 0x10 ) >> 1 ) +
	          ( ( *bp & 0x20 ) >> 3 ) +
	          ( ( *bp & 0x40 ) >> 5 ) +
	          ( ( *bp & 0x80 ) >> 7 );
/*	    col = *bp; */
	    putc( col, pstream);
		bp++;
	}
    }

/* Then pad */
	{
        char pix = 0;
        int x = 0;
        /* Already padded to the byte */
        /* Pad by 4 - ( width % 4 ) bytes if witdth % 4 != 0 */
        if( ( ( ( pdev->width + 7 ) >> 3 ) % 4 ) != 0 ) {
            for( x = 0; x < 4 - ( ( ( pdev->width + 7 ) >> 3 ) % 4 ); x++ ) {
                putc( pix, pstream );
            }
        }
    }

    return 0;
}
static int
spr1_print_page(gx_device_printer * pdev, FILE * pstream)
{
    return spr_print_page_loop(pdev, bdev->magic, pstream, spr1_print_row);
}

/* Print a gray-mapped page. */
static int
spr8m_print_row(gx_device_printer * pdev, byte * data, int depth,
	      FILE * pstream)
{				/* Note that bpp <= 8 for raw format, bpp <= 16 for plain. */
    uint mask = (1 << depth) - 1;
    byte *bp;
    uint x;
    int shift;

    if (bdev->is_raw && depth == 8)
	fwrite(data, 1, pdev->width, pstream);
    else
	for (bp = data, x = 0, shift = 8 - depth; x < pdev->width;) {
	    uint pixel;

/*	    if (shift < 0) { */	/* bpp = 16 */
/*		pixel = ((uint) * bp << 8) + bp[1];
		bp += 2;
	    } else */ {
		pixel = (*bp >> shift) & mask;
		if ((shift -= depth) < 0)
		    bp++, shift += 8;
	    }
	    ++x;
/*	    if (bdev->is_raw) */
		putc(pixel, pstream);
/*	    else
		fprintf(pstream, "%d%c", pixel,
			(x == pdev->width || !(x & 15) ? '\n' : ' ')); */
	}
	{
        char pix = 0;
        /* Pad by 4 - ( width % 4 ) bytes if witdth % 4 != 0 */
        if( ( pdev->width % 4 ) != 0 ) {
            for( x = 0; x < ( 4 - ( pdev->width % 4 ) ); x++ ) {
                putc( pix, pstream );
            }
        }
    }
    return 0;
}
static int
xspr_spr1_print_row(gx_device_printer * pdev, byte * data, int depth,
		  FILE * pstream)
{				/* Compress a spr8m or spr8c row to a spr1 row. */
    /* This doesn't have to be very fast. */
    /* Note that we have to invert the data as well. */
    int delta = (depth + 7) >> 3;
    byte *src = data + delta - 1;	/* always big-endian */
    byte *dest = data;
    int x;
    byte out_mask = 0x80;
    byte out = 0;

    if (depth >= 8) {		/* One or more bytes per source pixel. */
	for (x = 0; x < pdev->width; x++, src += delta) {
	    if (!(*src & 1))
		out |= out_mask;
	    out_mask >>= 1;
	    if (!out_mask)
		out_mask = 0x80,
		    *dest++ = out,
		    out = 0;
	}
    } else {			/* Multiple source pixels per byte. */
	byte in_mask = 0x100 >> depth;

	for (x = 0; x < pdev->width; x++) {
	    if (!(*src & in_mask))
		out |= out_mask;
	    in_mask >>= depth;
	    if (!in_mask)
		in_mask = 0x100 >> depth,
		    src++;
	    out_mask >>= 1;
	    if (!out_mask)
		out_mask = 0x80,
		    *dest++ = out,
		    out = 0;
	}
    }
    if (out_mask != 0x80)
	*dest = out;
    return spr1_print_row(pdev, data, 1, pstream);
}
static int
spr8m_print_page(gx_device_printer * pdev, FILE * pstream)
{
    return (bdev->uses_color == 0 && bdev->optimize ?
	    spr_print_page_loop(pdev, bdev->magic - 1, pstream,
				xspr_spr1_print_row) :
	    spr_print_page_loop(pdev, bdev->magic, pstream,
				spr8m_print_row));
}

/* Print a color-mapped page. */
static int
mspr8m_print_row(gx_device_printer * pdev, byte * data, int depth,
	       FILE * pstream, bool color)
{   /* If color=false, write only one value per pixel; */
    /* if color=true, write 3 values per pixel. */
    /* Note that depth <= 24 for raw format, depth <= 32 for plain. */
    uint bpe = depth / 3;	/* bits per r/g/b element */
    uint mask = (1 << bpe) - 1;
    byte *bp;
    uint x;
    uint eol_mask = (color ? 7 : 15);
    int shift;

/*    if (bdev->is_raw && depth == 24 && color)
	fwrite(data, 1, pdev->width * (depth / 8), pstream);
    else */
	for (bp = data, x = 0, shift = 8 - depth; x < pdev->width;) {
	    bits32 pixel = 0;
	    uint r, g, b;

	    switch (depth >> 3) {
		case 4:
		    pixel = 0; /*(bits32) * bp << 24; */ /* It's reserved */
		    bp++;
		    /* falls through */
		case 3:
		    pixel += (bits32) * bp << 16;
		    bp++;
		    /* falls through */
		case 2:
		    pixel += (uint) * bp << 8;
		    bp++;
		    /* falls through */
		case 1:
		    pixel += *bp;
		    bp++;
		    break;
		case 0:	/* bpp == 4, bpe == 1 */
/*		    pixel = *bp >> shift;
		    if ((shift -= depth) < 0)
			bp++, shift += 8; */ /* I don't think I care about this */
/*		    fprintf( stderr, "bpp = 4 or bpe = 1 in mspr8m_print_row\n" ); */
		    break;
	    }
	    ++x;
/*	    b = pixel & mask;
	    pixel >>= bpe;
	    g = pixel & mask;
	    pixel >>= bpe;
	    r = pixel & mask; */
/*	    if (bdev->is_raw) { */
	       /* I think I'm doing either 32bpp or 8bpp */
		if (color) {
            fwrite( &pixel, 1, sizeof( int ), pstream );
            /* In words so no padding */
		}
        else {
            char pix = pixel & mask;
            putc( pix, pstream );
        }
/*	    } else {
		if (color)
		    fprintf(pstream, "%d %d ", r, g);
		fprintf(pstream, "%d%c", b,
			(x == pdev->width || !(x & eol_mask) ?
			 '\n' : ' '));
	    } */
	}

    if( !color ) {
        char pix = 0;
        /* Pad by 4 - ( width % 4 ) bytes if witdth % 4 != 0 */
        if( ( pdev->width % 4 ) != 0 ) {
            for( x = 0; x < ( 4 - ( pdev->width % 4 ) ); x++ ) {
                putc( pix, pstream );
            }
        }
    }

    return 0;
}
static int
spr8c_print_row(gx_device_printer * pdev, byte * data, int depth,
	      FILE * pstream)
{
    return mspr8m_print_row(pdev, data, depth, pstream, true);
}
static int
spr8c_spr8m_print_row(gx_device_printer * pdev, byte * data, int depth,
		  FILE * pstream)
{
    return mspr8m_print_row(pdev, data, depth, pstream, false);
}
static int
spr8c_print_page(gx_device_printer * pdev, FILE * pstream)
{
    return (bdev->uses_color >= 2 || !bdev->optimize ?
	    spr_print_page_loop(pdev, bdev->magic, pstream,
				spr8c_print_row) :
	    bdev->uses_color == 1 ?
	    spr_print_page_loop(pdev, bdev->magic - 1, pstream,
				spr8c_spr8m_print_row) :
	    spr_print_page_loop(pdev, bdev->magic - 2, pstream,
				xspr_spr1_print_row));
}

/* Print a faux CMYK page. */
/* Print a row where each pixel occupies 4 bits (depth == 4). */
/* In this case, we also know pdev->color_info.max_color == 1. */
static int
sprk_print_row_4(gx_device_printer * pdev, byte * data, int depth,
		FILE * pstream)
{
    byte *bp;
    uint x;
    int shift;
    byte rv[16], gv[16], bv[16], i;

    /* Precompute all the possible pixel values. */
    for (i = 0; i < 16; ++i) {
	gx_color_value rgb[3];

	sprk_map_color_rgb((gx_device *) pdev, (gx_color_index) i, rgb);
	rv[i] = rgb[0] / gx_max_color_value;
	gv[i] = rgb[1] / gx_max_color_value;
	bv[i] = rgb[2] / gx_max_color_value;
    }
    for (bp = data, x = 0, shift = 4; x < pdev->width;) {
	int pixel = (*bp >> shift) & 0xf;
	int r = rv[pixel], g = gv[pixel], b = bv[pixel];

	shift ^= 4;
	bp += shift >> 2;
	++x;
	if (bdev->is_raw) {
	    putc(r, pstream);
	    putc(g, pstream);
	    putc(b, pstream);
	} else {
	    fprintf(pstream, "%d %d %d%c", r, g, b,
		    (x == pdev->width || !(x & 7) ?
		     '\n' : ' '));
	}
    }
    return 0;
}
/* Print a row where each pixel occupies 1 or more bytes (depth >= 8). */
static int
sprk_print_row(gx_device_printer * pdev, byte * data, int depth,
	      FILE * pstream)
{
    byte *bp;
    uint x;
    ulong max_value = pdev->color_info.max_color;

    for (bp = data, x = 0; x < pdev->width;) {
	bits32 pixel = 0;
	gx_color_value rgb[3];
	uint r, g, b;

	switch (depth >> 3) {
	    case 4:
		pixel = (bits32) * bp << 24;
		bp++;
		/* falls through */
	    case 3:
		pixel += (bits32) * bp << 16;
		bp++;
		/* falls through */
	    case 2:
		pixel += (uint) * bp << 8;
		bp++;
		/* falls through */
	    case 1:
		pixel += *bp;
		bp++;
	}
	++x;
	sprk_map_color_rgb((gx_device *) pdev, pixel, rgb);
	r = rgb[0] * max_value / gx_max_color_value;
	g = rgb[1] * max_value / gx_max_color_value;
	b = rgb[2] * max_value / gx_max_color_value;
	if (bdev->is_raw) {
	    putc(r, pstream);
	    putc(g, pstream);
	    putc(b, pstream);
	} else {
	    fprintf(pstream, "%d %d %d%c", r, g, b,
		    (x == pdev->width || !(x & 7) ?
		     '\n' : ' '));
	}
    }
    return 0;
}
static int
sprk_print_page(gx_device_printer * pdev, FILE * pstream)
{
    return spr_print_page_loop(pdev, bdev->magic, pstream,
			       (pdev->color_info.depth < 8 ?
				sprk_print_row_4 :
				sprk_print_row));
}
