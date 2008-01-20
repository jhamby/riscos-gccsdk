--- glib-2.0.pc.in.orig	2008-01-20 08:55:19.000000000 -0800
+++ glib-2.0.pc.in	2008-01-20 08:55:31.000000000 -0800
@@ -10,6 +10,6 @@
 Name: GLib
 Description: C Utility Library
 Version: @VERSION@
-Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ @ICONV_LIBS@
+Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ @ICONV_LIBS@ -lpcre
 Cflags: -I${includedir}/glib-2.0 -I${libdir}/glib-2.0/include 
 
