--- gnu/fpending.c.orig	2018-04-30 05:45:06.000000000 +1200
+++ gnu/fpending.c	2020-10-08 16:03:41.417639780 +1300
@@ -51,6 +51,8 @@
   return fp->__bufp - fp->__buffer;
 #elif defined EPLAN9                 /* Plan9 */
   return fp->wp - fp->buf;
+#elif defined __riscos__
+  return fp->o_ptr - fp->o_base;
 #else
 # error "Please port gnulib fpending.c to your platform!"
   return 1;
