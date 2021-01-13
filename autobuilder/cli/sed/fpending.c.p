--- lib/fpending.c.orig	2021-01-13 19:16:58.976233299 +0000
+++ lib/fpending.c	2021-01-13 19:18:20.206611794 +0000
@@ -55,6 +55,8 @@
   return fp->__bufp - fp->__buffer;
 #elif defined EPLAN9                 /* Plan9 */
   return fp->wp - fp->buf;
+#elif defined __riscos__             /* RISC OS */
+  return fp->o_ptr - fp->o_base;
 #else
 # error "Please port gnulib fpending.c to your platform!"
   return 1;
