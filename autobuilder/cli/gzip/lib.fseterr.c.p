--- lib/fseterr.c.orig	2021-10-09 11:24:42.554979893 +0100
+++ lib/fseterr.c	2021-10-09 11:25:19.826424900 +0100
@@ -53,6 +53,8 @@
 #elif defined EPLAN9                /* Plan9 */
   if (fp->state != 0 /* CLOSED */)
     fp->state = 5 /* ERR */;
+#elif defined __riscos__
+    fp->__error = 1;
 #elif 0                             /* unknown  */
   /* Portable fallback, based on an idea by Rich Felker.
      Wow! 6 system calls for something that is just a bit operation!
