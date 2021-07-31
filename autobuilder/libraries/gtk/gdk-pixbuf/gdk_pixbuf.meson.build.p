--- gdk-pixbuf/meson.build.orig	2019-02-28 16:22:57.000000000 +0000
+++ gdk-pixbuf/meson.build	2020-08-04 11:15:16.798797348 +0100
@@ -26,6 +26,9 @@
 
 gdk_pixbuf_inc = include_directories('.')
 
+gdk_pixbuf_riscos_abi = get_option('riscos_abi')
+gdk_pixbuf_libdir = '/SharedLibs:lib/' + gdk_pixbuf_riscos_abi
+
 gdk_pixbuf_cflags = [
   '-DG_LOG_STRUCTURED=1',
   '-DG_LOG_DOMAIN="GdkPixbuf"',
