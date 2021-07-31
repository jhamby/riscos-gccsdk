--- src/cairo-xlib-surface.c.orig	2013-05-29 22:22:45.537694565 +0100
+++ src/cairo-xlib-surface.c	2013-05-29 22:23:41.049563171 +0100
@@ -67,9 +67,11 @@

 #include <X11/Xutil.h> /* for XDestroyImage */

 #include <X11/extensions/XShm.h>
+#ifndef __riscos__
 #include <sys/ipc.h>
 #include <sys/shm.h>
+#endif

 #define XLIB_COORD_MAX 32767

