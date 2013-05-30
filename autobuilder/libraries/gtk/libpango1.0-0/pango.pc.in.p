--- pango.pc.in.orig	2013-05-30 15:35:21.293300074 +0100
+++ pango.pc.in	2013-05-30 15:36:58.125378180 +0100
@@ -10,5 +10,5 @@
 Version: @VERSION@
 Requires: glib-2.0 gobject-2.0
 Requires.private: gmodule-no-export-2.0
-Libs: -L${libdir} -lpango-@PANGO_API_VERSION@ @PKGCONFIG_MATH_LIBS@
+Libs: -L${libdir} -lpango-@PANGO_API_VERSION@ @PKGCONFIG_MATH_LIBS@ -lffi -lgmodule-2.0
 Cflags: -I${includedir}/pango-1.0
