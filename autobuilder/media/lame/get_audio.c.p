--- frontend/get_audio.c.orig	2012-01-01 18:03:49.000000000 +0000
+++ frontend/get_audio.c	2012-01-01 18:05:22.000000000 +0000
@@ -64,8 +64,8 @@
 #include <math.h>
 
 #if defined(__riscos__)
-# include <kernel.h>
-# include <sys/swis.h>
+# include <swis.h>
+# include <sys/stat.h>
 #elif defined(_WIN32)
 # include <sys/types.h>
 # include <sys/stat.h>
@@ -521,7 +521,7 @@
                     break;
                 }
             }
-            SetFiletype(out_path, decode ? 0xFB1 /*WAV*/ : 0x1AD /*AMPEG*/);
+//            SetFiletype(out_path, decode ? 0xFB1 /*WAV*/ : 0x1AD /*AMPEG*/);
             free(out_path);
         }
 #else
