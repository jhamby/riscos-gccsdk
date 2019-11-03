--- include/mupdf/fitz/write-pixmap.h.orig	2019-11-03 16:56:36.921958585 +0000
+++ include/mupdf/fitz/write-pixmap.h	2019-11-03 17:00:55.385958849 +0000
@@ -82,6 +82,10 @@
 void fz_write_pixmap_as_pnm(fz_context *ctx, fz_output *out, fz_pixmap *pixmap);
 fz_band_writer *fz_new_pnm_band_writer(fz_context *ctx, fz_output *out);
 
+void fz_save_pixmap_as_spr(fz_context *ctx, fz_pixmap *pixmap, const char *filename);
+void fz_write_pixmap_as_spr(fz_context *ctx, fz_output *out, fz_pixmap *pixmap);
+fz_band_writer *fz_new_spr_band_writer(fz_context *ctx, fz_output *out);
+
 void fz_save_pixmap_as_pam(fz_context *ctx, fz_pixmap *pixmap, const char *filename);
 void fz_write_pixmap_as_pam(fz_context *ctx, fz_output *out, fz_pixmap *pixmap);
 fz_band_writer *fz_new_pam_band_writer(fz_context *ctx, fz_output *out);
