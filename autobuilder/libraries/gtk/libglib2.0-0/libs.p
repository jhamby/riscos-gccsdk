--- glib-2.0.pc.in.orig	2019-01-21 13:38:50.000000000 +0000
+++ glib-2.0.pc.in	2020-08-02 17:55:12.121956360 +0100
@@ -12,6 +12,6 @@
 Description: C Utility Library
 Version: @VERSION@
 Requires.private: @PCRE_REQUIRES@
-Libs: -L${libdir} -lglib-2.0 @INTLLIBS@
+Libs: -L${libdir} -lglib-2.0 @INTLLIBS@ -lpcre -ldl
 Libs.private: @G_THREAD_LIBS@ @G_LIBS_EXTRA@ @PCRE_LIBS@ @INTLLIBS@ @ICONV_LIBS@ @CARBON_LIBS@ @COCOA_LIBS@
 Cflags: -I${includedir}/glib-2.0 -I${libdir}/glib-2.0/include @GLIB_EXTRA_CFLAGS@
