--- src/cairoint.h.orig	2016-12-08 01:26:33.000000000 +0000
+++ src/cairoint.h	2017-04-03 15:35:41.618349758 +0100
@@ -647,6 +647,12 @@
 
 #endif
 
+#if CAIRO_HAS_RISCOS_FONT
+
+extern const cairo_private struct _cairo_font_face_backend _cairo_riscos_font_face_backend;
+
+#endif
+
 #define CAIRO_EXTEND_SURFACE_DEFAULT CAIRO_EXTEND_NONE
 #define CAIRO_EXTEND_GRADIENT_DEFAULT CAIRO_EXTEND_PAD
 #define CAIRO_FILTER_DEFAULT CAIRO_FILTER_GOOD
