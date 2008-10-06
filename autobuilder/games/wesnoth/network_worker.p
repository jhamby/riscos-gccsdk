--- src/network_worker.hpp.orig	2008-07-11 14:11:59.751000000 +0100
+++ src/network_worker.hpp	2008-07-11 14:12:31.379000000 +0100
@@ -19,9 +19,11 @@
 // It is linux only extension
 #ifdef __GNUC_PREREQ
 #if __GNUC_PREREQ(2,1)
+#ifndef __riscos__
 #define USE_SENDFILE
 #endif
 #endif
+#endif
 
 #include <map>
 #include <vector>
