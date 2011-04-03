--- frontend/get_audio.c.orig	2011-02-18 23:55:08.000000000 +0000
+++ frontend/get_audio.c	2011-02-18 23:58:48.000000000 +0000
@@ -227,7 +227,7 @@
                 *p = '.';
                 break;
             }
-        SetFiletype(outPath, decode ? 0xFB1 /*WAV*/ : 0x1AD /*AMPEG*/);
+/*        SetFiletype(outPath, decode ? 0xFB1  : 0x1AD ); */
 #else
         (void) decode;
 #endif
