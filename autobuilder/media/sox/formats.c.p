--- src/formats.c.orig	2019-10-19 12:44:00.582257160 +0100
+++ src/formats.c	2019-10-19 12:44:29.590257190 +0100
@@ -417,7 +417,9 @@
   /* To fix this #error, either simply remove the #error line and live without
    * file-type detection with pipes, or add support for your compiler in the
    * lines above.  Test with cat monkey.wav | ./sox --info - */
+#ifndef __riscos__
   #error FIX NEEDED HERE
+#endif
   #define NO_REWIND_PIPE
   (void)fp;
 #endif
