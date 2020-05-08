--- include/mupdf/fitz/write-pixmap.h.orig	2020-05-08 20:18:10.759776336 +0100
+++ include/mupdf/fitz/write-pixmap.h	2020-05-08 20:21:41.119545268 +0100
@@ -191,6 +191,10 @@
 */
 fz_band_writer *fz_new_pnm_band_writer(fz_context *ctx, fz_output *out);
 
+void fz_save_pixmap_as_spr(fz_context *ctx, fz_pixmap *pixmap, const char *filename);
+void fz_write_pixmap_as_spr(fz_context *ctx, fz_output *out, fz_pixmap *pixmap);
+fz_band_writer *fz_new_spr_band_writer(fz_context *ctx, fz_output *out);
+
 /**
 	Save a pixmap as a pnm (greyscale, rgb or cmyk, with or without
 	alpha).
