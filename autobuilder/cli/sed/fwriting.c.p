--- lib/fwriting.c.orig	2012-09-13 07:58:19.000000000 +0100
+++ lib/fwriting.c	2014-02-27 12:30:47.000000000 +0000
@@ -56,6 +56,9 @@
   if (fp->state == 0 /* CLOSED */ || fp->state == 3 /* RD */)
     return 0;
   return (fp->state == 4 /* WR */ && (fp->bufl == 0 || fp->wp < fp->rp));
+#elif defined __riscos__
+  if (!fp->__mode.__bits.__read) return 1;
+  return  (fp->__mode.__bits.__write) != 0;
 #else
 # error "Please port gnulib fwriting.c to your platform!"
 #endif
