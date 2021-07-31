--- lib/fpending.c.orig	2019-02-05 18:26:35.000000000 +1300
+++ lib/fpending.c	2020-09-12 23:55:59.340138697 +1200
@@ -55,6 +55,8 @@
   return fp->__bufp - fp->__buffer;
 #elif defined EPLAN9                 /* Plan9 */
   return fp->wp - fp->buf;
+#elif defined __riscos__
+  return fp->o_ptr - fp->o_base;
 #else
 # error "Please port gnulib fpending.c to your platform!"
   return 1;
