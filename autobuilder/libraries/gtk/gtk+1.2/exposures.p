--- gdk/gdkevents.c.orig	2005-03-01 19:34:39.624853128 +0000
+++ gdk/gdkevents.c	2005-03-01 19:34:48.700000000 +0000
@@ -430,6 +430,7 @@
 gdk_compress_exposures (XEvent    *xevent,
 			GdkWindow *window)
 {
+#if 0
   gint nrects = 1;
   gint count = 0;
   GdkRectangle rect1;
@@ -539,6 +540,7 @@
   xevent->xexpose.y = rect1.y;
   xevent->xexpose.width = rect1.width;
   xevent->xexpose.height = rect1.height;
+#endif  
 }
 
 /*************************************************************
