--- xft.pc.in.orig	2008-01-17 20:02:41.000000000 -0800
+++ xft.pc.in	2008-01-17 20:02:51.000000000 -0800
@@ -12,4 +12,4 @@
 Requires: xproto, fontconfig
 Cflags: -I${includedir} ${freetypecflags}
 Libs: -L${libdir} -lXft
-Libs.private: -lX11 ${xrenderlibs} ${freetypelibs}
+Libs.private: -lCX11 -lDesk ${xrenderlibs} ${freetypelibs}
