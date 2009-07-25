--- lib/fseterr.c.orig	2009-05-03 22:46:33.000000000 -0700
+++ lib/fseterr.c	2009-07-25 08:58:13.000000000 -0700
@@ -43,6 +43,8 @@
   fp->_Mode |= 0x200 /* _MERR */;
 #elif defined __MINT__              /* Atari FreeMiNT */
   fp->__error = 1;
+#elif defined __riscos__
+  fp->__error = 1;
 #elif 0                             /* unknown  */
   /* Portable fallback, based on an idea by Rich Felker.
      Wow! 6 system calls for something that is just a bit operation!
