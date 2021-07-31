--- lib/fpending.c.orig	2020-02-25 12:36:03.864571507 +0000
+++ lib/fpending.c	2020-02-25 12:45:59.260459202 +0000
@@ -55,6 +55,8 @@
   return fp->__bufp - fp->__buffer;
 #elif defined EPLAN9                 /* Plan9 */
   return fp->wp - fp->buf;
+#elif defined __riscos__                /* RISC OS */
+  return (fp->i_ptr - fp->i_base);
 #else
 # error "Please port gnulib fpending.c to your platform!"
   return 1;
