--- lib/freading.c.orig	2009-05-03 22:46:33.000000000 -0700
+++ lib/freading.c	2009-07-25 08:56:38.000000000 -0700
@@ -56,6 +56,8 @@
 # else
   return (fp->__buffer < fp->__get_limit /*|| fp->__bufp == fp->__put_limit ??*/);
 # endif
+#elif defined __riscos__
+  return (fp->__mode.__bits.__read) != 0;
 # else
 # error "Please port gnulib freading.c to your platform!"
 # endif
