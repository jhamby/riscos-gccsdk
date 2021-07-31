--- bitmap.h.orig	2002-01-31 22:20:15.000000000 +0100
+++ bitmap.h	2012-05-06 11:36:22.000000000 +0200
@@ -102,8 +102,14 @@
 
 void b_makebitmap __PROTO((unsigned int x, unsigned int y, unsigned int planes));
 void b_freebitmap __PROTO((void));
-void b_setpixel __PROTO((unsigned int x, unsigned int y, unsigned int value));
+/* BB change 20010510
+   change next function to pointer so other underlying bitmaps
+   can be used.
+*/
+extern void (*b_setpixel) __PROTO((unsigned int x, unsigned int y, unsigned int value));
+
 unsigned int b_getpixel __PROTO((unsigned int x, unsigned int y));
+
 void b_line __PROTO((unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2));
 void b_setmaskpixel __PROTO((unsigned int x, unsigned int y, unsigned int value));
 /* void b_putc __PROTO((unsigned int x, unsigned int y, char c, unsigned int angle)); */
