--- gl/lib/freading.c.orig	2020-11-30 14:53:24.132273444 +0000
+++ gl/lib/freading.c	2020-11-30 14:55:36.310876816 +0000
@@ -68,6 +68,8 @@
   if (fp->state == 0 /* CLOSED */ || fp->state == 4 /* WR */)
     return 0;
   return (fp->state == 3 /* RD */ && (fp->bufl == 0 || fp->rp < fp->wp));
+# elif defined __riscos__
+  return (fp->__mode.__bits.__read) != 0;
 # else
 #  error "Please port gnulib freading.c to your platform!"
 # endif
