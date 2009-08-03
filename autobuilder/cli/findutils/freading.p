--- gnulib/lib/freading.c.orig	2007-11-07 23:32:20.000000000 -0800
+++ gnulib/lib/freading.c	2008-01-27 20:14:04.000000000 -0800
@@ -42,6 +42,8 @@
 #elif defined __QNX__               /* QNX */
   return ((fp->_Mode & 0x2 /* _MOPENW */) == 0
 	  || (fp->_Mode & 0x1000 /* _MREAD */) != 0);
+#elif defined __riscos__
+  return (fp->__mode.__bits.__read) != 0;
 #else
  #error "Please port gnulib freading.c to your platform!"
 #endif
