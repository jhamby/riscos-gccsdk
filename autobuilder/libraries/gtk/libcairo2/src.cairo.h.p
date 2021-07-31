--- src/cairo.h.orig	2018-10-18 01:21:03.000000000 +0100
+++ src/cairo.h	2019-11-04 11:45:59.706911492 +0000
@@ -356,6 +356,9 @@
     CAIRO_STATUS_FREETYPE_ERROR,
     CAIRO_STATUS_WIN32_GDI_ERROR,
     CAIRO_STATUS_TAG_ERROR,
+#ifdef __riscos__
+    CAIRO_STATUS_RISCOS_ERROR,
+#endif
 
     CAIRO_STATUS_LAST_STATUS
 } cairo_status_t;
@@ -1575,6 +1578,10 @@
     CAIRO_FONT_TYPE_WIN32,
     CAIRO_FONT_TYPE_QUARTZ,
     CAIRO_FONT_TYPE_USER
+#ifdef __riscos__
+    ,
+    CAIRO_FONT_TYPE_RISCOS
+#endif
 } cairo_font_type_t;
 
 cairo_public cairo_font_type_t
@@ -2420,6 +2427,10 @@
     CAIRO_SURFACE_TYPE_SKIA,
     CAIRO_SURFACE_TYPE_SUBSURFACE,
     CAIRO_SURFACE_TYPE_COGL
+#ifdef __riscos__
+    ,
+    CAIRO_SURFACE_TYPE_RISCOS_SPRITE
+#endif
 } cairo_surface_type_t;
 
 cairo_public cairo_surface_type_t
