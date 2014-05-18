--- pango-view/viewer-cairo.c.orig	2012-08-24 20:08:22.000000000 +0100
+++ pango-view/viewer-cairo.c	2013-06-23 16:24:13.000000000 +0100
@@ -28,6 +28,10 @@
 #include <string.h>
 
 
+#ifdef __riscos__
+#undef HAVE_CAIRO_XLIB
+#undef HAVE_X
+#endif
 
 #ifdef HAVE_CAIRO_XLIB
 #ifdef HAVE_XFT
