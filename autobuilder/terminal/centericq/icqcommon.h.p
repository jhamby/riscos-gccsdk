--- src/icqcommon.h.old	2003-02-07 12:25:55.000000000 +0000
+++ src/icqcommon.h	2003-02-07 12:26:32.000000000 +0000
@@ -25,11 +25,13 @@
 
 #include <sys/param.h>
 
+#ifndef __riscos__
 #ifdef BSD
 #include <sys/mount.h>
 #else
 #include <sys/vfs.h>
 #endif
+#endif
 
 #define PERIOD_RECONNECT        40
 
