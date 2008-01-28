--- lib/freading.c.orig	2008-01-27 18:52:56.000000000 -0800
+++ lib/freading.c	2008-01-27 18:57:33.000000000 -0800
@@ -40,6 +40,8 @@
   return (fp->_flag & _IOREAD) != 0;
 #elif defined __UCLIBC__            /* uClibc */
   return (fp->__modeflags & (__FLAG_READONLY | __FLAG_READING)) != 0;
+#elif defined __riscos__
+  return (fp->__mode.__bits.__read) != 0;
 #else
  #error "Please port gnulib freading.c to your platform!"
 #endif
