--- lib/freading.c.orig	2017-01-01 02:54:41.000000000 +1300
+++ lib/freading.c	2020-09-08 01:08:57.999733977 +1200
@@ -67,6 +67,8 @@
   if (fp->state == 0 /* CLOSED */ || fp->state == 4 /* WR */)
     return 0;
   return (fp->state == 3 /* RD */ && (fp->bufl == 0 || fp->rp < fp->wp));
+# elif defined __riscos__
+  return (fp->__mode.__bits.__read) != 0; 
 # else
 #  error "Please port gnulib freading.c to your platform!"
 # endif
