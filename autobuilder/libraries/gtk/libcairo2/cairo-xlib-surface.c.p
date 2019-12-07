--- src/cairo-xlib-surface.c.orig	2016-12-08 01:26:33.000000000 +0000
+++ src/cairo-xlib-surface.c	2017-04-03 15:35:41.642349758 +0100
@@ -69,8 +69,10 @@
 #include <X11/Xutil.h> /* for XDestroyImage */
 
 #include <X11/extensions/XShm.h>
+#ifndef __riscos__
 #include <sys/ipc.h>
 #include <sys/shm.h>
+#endif
 
 #define XLIB_COORD_MAX 32767
 
