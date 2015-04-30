--- lib/freading.c.orig	2015-02-19 12:52:16.525992764 +0000
+++ lib/freading.c	2015-02-27 13:03:55.334972398 +0000
@@ -66,6 +66,8 @@
   if (fp->state == 0 /* CLOSED */ || fp->state == 4 /* WR */)
     return 0;
   return (fp->state == 3 /* RD */ && (fp->bufl == 0 || fp->rp < fp->wp));
+#elif defined __riscos__
+  return (fp->__mode.__bits.__read) != 0;
 # else
 #  error "Please port gnulib freading.c to your platform!"
 # endif
