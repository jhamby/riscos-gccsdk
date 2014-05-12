--- src/cairo.h.orig	2012-03-29 11:05:32.000000000 +0100
+++ src/cairo.h	2013-05-08 19:59:16.000000000 +0100
@@ -345,6 +345,9 @@
     CAIRO_STATUS_DEVICE_ERROR,
     CAIRO_STATUS_INVALID_MESH_CONSTRUCTION,
     CAIRO_STATUS_DEVICE_FINISHED,
+#ifdef __riscos__
+    CAIRO_STATUS_RISCOS_ERROR,
+#endif
 
     CAIRO_STATUS_LAST_STATUS
 } cairo_status_t;
@@ -1546,6 +1549,10 @@
     CAIRO_FONT_TYPE_WIN32,
     CAIRO_FONT_TYPE_QUARTZ,
     CAIRO_FONT_TYPE_USER
+#ifdef __riscos__
+    ,
+    CAIRO_FONT_TYPE_RISCOS
+#endif
 } cairo_font_type_t;
 
 cairo_public cairo_font_type_t
@@ -2383,6 +2390,10 @@
     CAIRO_SURFACE_TYPE_SKIA,
     CAIRO_SURFACE_TYPE_SUBSURFACE,
     CAIRO_SURFACE_TYPE_COGL
+#ifdef __riscos__
+    ,
+    CAIRO_SURFACE_TYPE_RISCOS_SPRITE
+#endif
 } cairo_surface_type_t;
 
 cairo_public cairo_surface_type_t
