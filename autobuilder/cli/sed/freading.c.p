--- gnulib-tests/freading.c.orig	2021-01-13 19:13:00.120930405 +0000
+++ gnulib-tests/freading.c	2021-01-13 19:14:57.382648355 +0000
@@ -68,6 +68,8 @@
   if (fp->state == 0 /* CLOSED */ || fp->state == 4 /* WR */)
     return 0;
   return (fp->state == 3 /* RD */ && (fp->bufl == 0 || fp->rp < fp->wp));
+# elif defined __riscos__            /* RISC OS */
+    return (fp->__mode.__bits.__read) != 0;
 # else
 #  error "Please port gnulib freading.c to your platform!"
 # endif
