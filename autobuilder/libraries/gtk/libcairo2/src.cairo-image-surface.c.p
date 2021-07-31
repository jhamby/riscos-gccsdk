--- src/cairo-image-surface.c.orig	2016-12-08 01:26:33.000000000 +0000
+++ src/cairo-image-surface.c	2017-04-03 15:35:41.646349758 +0100
@@ -97,6 +97,9 @@
     case PIXMAN_x2r10g10b10:
 	return CAIRO_FORMAT_RGB30;
     case PIXMAN_x8r8g8b8:
+#ifdef __riscos__
+    case PIXMAN_x8b8g8r8:
+#endif
 	return CAIRO_FORMAT_RGB24;
     case PIXMAN_a8:
 	return CAIRO_FORMAT_A8;
@@ -110,7 +113,11 @@
 #if PIXMAN_VERSION >= PIXMAN_VERSION_ENCODE(0,27,2)
     case PIXMAN_a8r8g8b8_sRGB:
 #endif
+#ifdef __riscos__
+    case PIXMAN_a8b8g8r8:/* case PIXMAN_x8b8g8r8:*/ case PIXMAN_r8g8b8:
+#else
     case PIXMAN_a8b8g8r8: case PIXMAN_x8b8g8r8: case PIXMAN_r8g8b8:
+#endif
     case PIXMAN_b8g8r8:   case PIXMAN_b5g6r5:
     case PIXMAN_a1r5g5b5: case PIXMAN_x1r5g5b5: case PIXMAN_a1b5g5r5:
     case PIXMAN_x1b5g5r5: case PIXMAN_a4r4g4b4: case PIXMAN_x4r4g4b4:
