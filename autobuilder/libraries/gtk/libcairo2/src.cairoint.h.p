--- src/cairoint.h.orig	2013-01-03 20:06:16.000000000 +0000
+++ src/cairoint.h	2013-04-26 19:47:55.000000000 +0100
@@ -621,6 +621,12 @@
 
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
