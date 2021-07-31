--- lib/freading.c.orig	2021-03-27 09:41:41.868098308 +0000
+++ lib/freading.c	2021-03-27 09:44:09.041221611 +0000
@@ -68,6 +68,8 @@
   if (fp->state == 0 /* CLOSED */ || fp->state == 4 /* WR */)
     return 0;
   return (fp->state == 3 /* RD */ && (fp->bufl == 0 || fp->rp < fp->wp));
+# elif defined __riscos__
+    return (fp->__mode.__bits.__read) != 0;
 # else
 #  error "Please port gnulib freading.c to your platform!"
 # endif
