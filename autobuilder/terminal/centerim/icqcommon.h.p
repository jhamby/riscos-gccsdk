--- src/icqcommon.h.orig	2008-01-22 21:15:12.000000000 -0800
+++ src/icqcommon.h	2008-01-22 21:15:28.000000000 -0800
@@ -35,7 +35,7 @@
 #elif defined BSD
 #include <sys/mount.h>
 #else
-#include <sys/vfs.h>
+/*#include <sys/vfs.h>*/
 #endif
 
 #define PERIOD_RECONNECT        40
