--- lib/freading.c.orig	2007-11-10 15:24:02.000000000 -0800
+++ lib/freading.c	2009-01-01 11:12:23.000000000 -0800
@@ -42,6 +42,8 @@
 #elif defined __QNX__               /* QNX */
   return ((fp->_Mode & 0x2 /* _MOPENW */) == 0
 	  || (fp->_Mode & 0x1000 /* _MREAD */) != 0);
+#elif defined __riscos__
+  return (fp->__mode.__bits.__read) != 0;
 #else
  #error "Please port gnulib freading.c to your platform!"
 #endif
