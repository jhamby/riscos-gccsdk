#include "mupdf/fitz.h"
#include <stdio.h>
#include <string.h>

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

/*
 * Write pixmap to SPR file (without alpha channel)
 */
static void
spr_write_header(fz_context *ctx, fz_band_writer *writer, const fz_colorspace *cs)
{
	fz_output *out = writer->out;
	int w = writer->w;
	int h = writer->h;
	int n = writer->n;
	int alpha = writer->alpha;

	int spr_size, spr_width, spr_right_bit, spr_mode;
        sprite_area spr_area;
	sprite_header spr_header;

	if (writer->s != 0)
		fz_throw(ctx, FZ_ERROR_GENERIC, "SPR writer cannot cope with spot colors");

	if (alpha)
		fz_throw(ctx, FZ_ERROR_GENERIC, "SPR writer cannot cope with alpha");

	n -= alpha;
	if (n != 1 && n != 3)
		fz_throw(ctx, FZ_ERROR_GENERIC, "pixmap must be grayscale or rgb to write as spr");

	strncpy( spr_header.name, "screen", 12 );
	spr_header.height = h - 1;
	spr_header.left_bit = 0;
	spr_header.image = sizeof(sprite_header);
	spr_header.mask = sizeof(sprite_header);
        spr_size = w * 4 * h; /* 4 bytes per word, per line */
	spr_width = w - 1;
	spr_right_bit = 31;
	spr_mode = ( 6 << 27 ) + ( 90 << 14 ) + ( 90 << 1 ) + 1;
	spr_header.size = spr_size + sizeof(sprite_header);
	spr_header.width = spr_width;
	spr_header.right_bit = spr_right_bit;
	spr_header.mode = spr_mode;
	spr_area.sprite_count = 1;
	spr_area.first = sizeof(sprite_area) + sizeof(int);
	spr_area.used = sizeof(sprite_area) + sizeof(int) + spr_size + sizeof(sprite_header);
	fz_write_data(ctx, out, (const void*)&spr_area, sizeof(sprite_area));
	fz_write_data(ctx, out, (const void*)&spr_header, sizeof(sprite_header));

}

static void
spr_write_band(fz_context *ctx, fz_band_writer *writer, int stride, int band_start, int band_height, const unsigned char *p)
{
	fz_output *out = writer->out;
	int w = writer->w;
	int h = writer->h;
	int n = writer->n;
	int x, y;
	int len;
	int end = band_start + band_height;

	if (n != 1 && n != 3)
		fz_throw(ctx, FZ_ERROR_GENERIC, "pixmap must be grayscale or rgb to write as spr");

	if (!out)
		return;

	if (end > h)
		end = h;
	end -= band_start;

	/* Tests show that writing single bytes out at a time
	 * is appallingly slow. We get a huge improvement
	 * by collating stuff into buffers first. */

for (y = 0; y <h; ++y) {
	for (x = 0; x < w; ++x) {
		fz_write_data(ctx, out, p, 3);
		fz_write_byte(ctx, out, 0);
		p += 3;
	}
}

#ifndef __riscos__
	while (end--)
	{
		len = w;
		fprintf(stdout, "width: %d\n", len);
		fflush(stdout);
		while (len)
		{
			int num_written = len;

			switch (n)
			{
			case 1:
				/* No collation required */
				fz_write_data(ctx, out, p, num_written);
				p += num_written;
				break;
			case 3:
				fz_write_data(ctx, out, p, num_written);
				fz_write_byte(ctx, out, 0);
				p += num_written;
				break;
			}
			len -= num_written;
		}
		p += stride - w*n;
	}
#endif
}

fz_band_writer *fz_new_spr_band_writer(fz_context *ctx, fz_output *out)
{
	fz_band_writer *writer = fz_new_band_writer(ctx, fz_band_writer, out);

	writer->header = spr_write_header;
	writer->band = spr_write_band;

	return writer;
}

void
fz_write_pixmap_as_spr(fz_context *ctx, fz_output *out, fz_pixmap *pixmap)
{
	fz_band_writer *writer = fz_new_spr_band_writer(ctx, out);
	fz_try(ctx)
	{
		fz_write_header(ctx, writer, pixmap->w, pixmap->h, pixmap->n, pixmap->alpha, 0, 0, 0, pixmap->colorspace, pixmap->seps);
		fz_write_band(ctx, writer, pixmap->stride, pixmap->h, pixmap->samples);
	}
	fz_always(ctx)
		fz_drop_band_writer(ctx, writer);
	fz_catch(ctx)
		fz_rethrow(ctx);
}

void
fz_save_pixmap_as_spr(fz_context *ctx, fz_pixmap *pixmap, const char *filename)
{
	fprintf(stderr, "Filename1a: %s\n", filename);
	fflush(stderr);
	fz_band_writer *writer = NULL;
	fz_output *out = fz_new_output_with_path(ctx, filename, 0);

	fz_var(writer);

	fz_try(ctx)
	{
		writer = fz_new_spr_band_writer(ctx, out);
		fz_write_header(ctx, writer, pixmap->w, pixmap->h, pixmap->n, pixmap->alpha, 0, 0, 0, pixmap->colorspace, pixmap->seps);
		fz_write_band(ctx, writer, pixmap->stride, pixmap->h, pixmap->samples);
		fz_close_output(ctx, out);
	}
	fz_always(ctx)
	{
		fz_drop_band_writer(ctx, writer);
		fz_drop_output(ctx, out);
	}
	fz_catch(ctx)
		fz_rethrow(ctx);
}
