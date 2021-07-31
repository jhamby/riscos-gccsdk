--- src/vc6.h.orig	2004-06-07 22:02:48.000000000 +0100
+++ src/vc6.h	2014-03-13 09:15:47.000000000 +0000
@@ -8,4 +8,10 @@
 #   pragma warning(disable:4800) // Disable "conversion from int to bool" compiler performance warning
 #endif
 
+#ifdef __riscos__
+// RISC OS GCC 4.7.4 compiler needs string.h included for the functions to be found
+#include "string.h"
 #endif
+
+#endif
+
