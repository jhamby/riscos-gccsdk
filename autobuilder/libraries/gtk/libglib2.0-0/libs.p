--- glib-2.0.pc.in.orig	2009-04-19 15:27:49.000000000 -0700
+++ glib-2.0.pc.in	2009-04-19 15:25:36.000000000 -0700
@@ -10,6 +10,6 @@
 Name: GLib
 Description: C Utility Library
 Version: @VERSION@
-Libs: -L${libdir} -lglib-2.0 @INTLLIBS@
+Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ -lpcre -ldl
 Libs.private: @ICONV_LIBS@ @GLIB_RT_LIBS@
 Cflags: -I${includedir}/glib-2.0 -I${libdir}/glib-2.0/include @GLIB_EXTRA_CFLAGS@
