--- include/unixconf.h.old	2003-01-31 21:31:47.000000000 +0000
+++ include/unixconf.h	2003-01-31 21:32:44.000000000 +0000
@@ -253,6 +253,9 @@
 #define SUSPEND		/* let ^Z suspend the game */
 #endif
 
+#ifdef __riscos__
+#include <time.h>
+#endif
 
 #if defined(BSD) || defined(ULTRIX)
 #include <sys/time.h>
