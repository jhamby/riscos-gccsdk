--- lib/fseterr.c.orig	2008-01-27 20:51:49.000000000 -0800
+++ lib/fseterr.c	2008-01-27 20:53:48.000000000 -0800
@@ -48,6 +48,8 @@
   fp->__modeflags |= __FLAG_ERROR;
 #elif defined __QNX__               /* QNX */
   fp->_Mode |= 0x200 /* _MERR */;
+#elif defined __riscos__
+  fp->__error = 1;
 #elif 0                             /* unknown  */
   /* Portable fallback, based on an idea by Rich Felker.
      Wow! 6 system calls for something that is just a bit operation!
