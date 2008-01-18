--- x11.pc.in.org	2006-12-16 14:39:54.000000000 -0800
+++ x11.pc.in	2006-12-16 14:40:11.000000000 -0800
@@ -11,5 +11,5 @@
 Requires: xproto kbproto
 Requires.private: xau xdmcp
 Cflags: -I${includedir} @XTHREAD_CFLAGS@
-Libs: -L${libdir} -lX11
+Libs: -L${libdir} -lCX11 -lDesk
 Libs.private: @XTHREADLIB@
