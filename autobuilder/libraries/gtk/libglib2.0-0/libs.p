--- glib-2.0.pc.in.orig	2012-03-12 00:42:41.000000000 +0000
+++ glib-2.0.pc.in	2012-07-30 23:54:44.000000000 +0100
@@ -11,6 +11,6 @@
 Description: C Utility Library
 Version: @VERSION@
 Requires.private: @PCRE_REQUIRES@
-Libs: -L${libdir} -lglib-2.0 @INTLLIBS@
+Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ -lpcre -ldl
 Libs.private: @G_THREAD_LIBS@ @G_LIBS_EXTRA@ @PCRE_LIBS@ @INTLLIBS@ @ICONV_LIBS@
 Cflags: -I${includedir}/glib-2.0 -I${libdir}/glib-2.0/include @GLIB_EXTRA_CFLAGS@
