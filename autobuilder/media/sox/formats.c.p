--- src/formats.c.orig	2011-09-23 21:42:30.000000000 +0100
+++ src/formats.c	2011-09-23 21:42:56.000000000 +0100
@@ -407,7 +407,9 @@
   /* To fix this #error, either simply remove the #error line and live without
    * file-type detection with pipes, or add support for your compiler in the
    * lines above.  Test with cat monkey.au | ./sox --info - */
+#ifndef __riscos__
   #error FIX NEEDED HERE
+#endif
   #define NO_REWIND_PIPE
   (void)fp;
 #endif
