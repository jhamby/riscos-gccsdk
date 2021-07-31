--- libmp3lame/lame.c.orig	2011-02-18 23:33:59.000000000 +0000
+++ libmp3lame/lame.c	2011-02-18 23:34:28.000000000 +0000
@@ -50,9 +50,9 @@
 #if defined(__FreeBSD__) && !defined(__alpha__)
 #include <floatingpoint.h>
 #endif
-#ifdef __riscos__
+/* #ifdef __riscos__
 #include "asmstuff.h"
-#endif
+#endif */
 
 #ifdef __sun__
 /* woraround for SunOS 4.x, it has SEEK_* defined here */
