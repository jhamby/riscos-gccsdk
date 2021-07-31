--- lib/fseterr.c.orig	2019-02-05 18:26:35.000000000 +1300
+++ lib/fseterr.c	2020-09-12 23:44:57.840051767 +1200
@@ -53,6 +53,8 @@
 #elif defined EPLAN9                /* Plan9 */
   if (fp->state != 0 /* CLOSED */)
     fp->state = 5 /* ERR */;
+#elif defined __riscos__
+    fp->__error = 1;   
 #elif 0                             /* unknown  */
   /* Portable fallback, based on an idea by Rich Felker.
      Wow! 6 system calls for something that is just a bit operation!
