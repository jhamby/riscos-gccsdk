--- mono/utils/mono-mmap.c.orig	2012-04-10 20:46:10.000000000 +0100
+++ mono/utils/mono-mmap.c	2012-04-10 20:46:57.000000000 +0100
@@ -612,3 +612,7 @@
 #endif // HAVE_SHM_OPEN
 
 #endif // HOST_WIN32
+
+#ifdef __riscos__
+#include "riscos-memory.c"
+#endif
