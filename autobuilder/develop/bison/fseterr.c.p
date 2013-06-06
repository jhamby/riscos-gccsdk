--- lib/fseterr.c.orig	2013-06-06 16:20:46.685373306 +0100
+++ lib/fseterr.c	2013-06-06 16:21:23.653306902 +0100
@@ -48,7 +48,7 @@
 #elif defined EPLAN9                /* Plan9 */
   if (fp->state != 0 /* CLOSED */)
     fp->state = 5 /* ERR */;
-#elif 0                             /* unknown  */
+#elif __riscos__                    /* unknown  */
   /* Portable fallback, based on an idea by Rich Felker.
      Wow! 6 system calls for something that is just a bit operation!
      Not activated on any system, because there is no way to repair FP when
