--- configure.ac.orig	2013-08-26 17:11:51.000000000 +0100
+++ configure.ac	2014-03-24 20:45:26.931119226 +0000
@@ -76,6 +76,17 @@
 
 dnl ===========================================================================
 
+CAIRO_ENABLE_SURFACE_BACKEND(riscos_sprite, RISC OS Sprite, auto, [
+  AC_CHECK_HEADER(unixlib/buildoptions.h,use_riscos_sprite=yes,[use_riscos_sprite="no (requires RISC OS GCCSDK Cross Compiler)"])
+  riscos_sprite_NONPKGCONFIG_LIBS=-lOSLib32
+])
+
+CAIRO_ENABLE_FONT_BACKEND(riscos_font, RISC OS, auto, [
+  AC_CHECK_HEADER(unixlib/buildoptions.h,use_riscos_font=yes,[use_riscos_font="no (requires RISC OS GCCSDK Cross Compiler)"])
+])
+
+dnl ===========================================================================
+
 CAIRO_ENABLE_SURFACE_BACKEND(xlib, Xlib, auto, [
   xlib_REQUIRES="x11 xext"
   PKG_CHECK_MODULES(xlib, $xlib_REQUIRES, ,
@@ -185,7 +196,7 @@
 dnl ===========================================================================
 
 CAIRO_ENABLE_SURFACE_BACKEND(qt, Qt, no, [
-  qt_REQUIRES="QtGui >= 4.4.0"
+  qt_REQUIRES="Qt5Widgets >= 4.4.0"
   PKG_CHECK_MODULES(qt, $qt_REQUIRES, ,
 		    [qt_REQUIRES=""
 		     use_qt="no (requires Qt4 development libraries)"
