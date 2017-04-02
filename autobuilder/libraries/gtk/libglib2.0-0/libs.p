--- glib-2.0.pc.in.orig	2017-04-01 20:02:42.484064630 +0100
+++ glib-2.0.pc.in	2017-04-01 20:15:41.472074725 +0100
@@ -11,6 +11,6 @@
 Description: C Utility Library
 Version: @VERSION@
 Requires.private: @PCRE_REQUIRES@
-Libs: -L${libdir} -lglib-2.0 @INTLLIBS@
+Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ -lpcre -ldl
 Libs.private: @G_THREAD_LIBS@ @G_LIBS_EXTRA@ @PCRE_LIBS@ @INTLLIBS@ @ICONV_LIBS@ @CARBON_LIBS@ @COCOA_LIBS@
 Cflags: -I${includedir}/glib-2.0 -I${libdir}/glib-2.0/include @GLIB_EXTRA_CFLAGS@
