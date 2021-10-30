--- src/gl/fpending.c.orig	2021-10-24 09:18:00.170519653 +0100
+++ src/gl/fpending.c	2021-10-24 09:18:59.454116868 +0100
@@ -56,6 +56,8 @@
   return fp->__bufp - fp->__buffer;
 #elif defined EPLAN9                 /* Plan9 */
   return fp->wp - fp->buf;
+#elif defined __riscos__
+  return fp->o_ptr - fp->o_base;
 #else
 # error "Please port gnulib fpending.c to your platform!"
   return 1;
