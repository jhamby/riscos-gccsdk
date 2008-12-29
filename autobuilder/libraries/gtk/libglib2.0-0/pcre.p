--- glib-2.0.pc.in.orig	2008-09-12 10:55:53.000000000 -0700
+++ glib-2.0.pc.in	2008-12-28 21:42:27.000000000 -0800
@@ -10,5 +10,5 @@
 Name: GLib
 Description: C Utility Library
 Version: @VERSION@
-Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ @ICONV_LIBS@
+Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ @ICONV_LIBS@ -lpcre
 Cflags: -I${includedir}/glib-2.0 -I${libdir}/glib-2.0/include @GLIB_EXTRA_CFLAGS@
