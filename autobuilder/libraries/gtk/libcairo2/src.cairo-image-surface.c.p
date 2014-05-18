--- src/cairo-image-surface.c.orig	2013-08-26 16:07:21.000000000 +0100
+++ src/cairo-image-surface.c	2014-05-17 17:48:04.379109485 +0100
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
@@ -107,7 +110,11 @@
 #if PIXMAN_VERSION >= PIXMAN_VERSION_ENCODE(0,22,0)
     case PIXMAN_r8g8b8a8: case PIXMAN_r8g8b8x8:
 #endif
+#ifdef __riscos__
+    case PIXMAN_a8b8g8r8:/* case PIXMAN_x8b8g8r8:*/ case PIXMAN_r8g8b8:
+#else
     case PIXMAN_a8b8g8r8: case PIXMAN_x8b8g8r8: case PIXMAN_r8g8b8:
+#endif
     case PIXMAN_b8g8r8:   case PIXMAN_b5g6r5:
     case PIXMAN_a1r5g5b5: case PIXMAN_x1r5g5b5: case PIXMAN_a1b5g5r5:
     case PIXMAN_x1b5g5r5: case PIXMAN_a4r4g4b4: case PIXMAN_x4r4g4b4:
