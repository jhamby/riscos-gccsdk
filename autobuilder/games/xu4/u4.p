--- src/u4.cpp.orig	2008-07-23 11:26:14.770534000 +0100
+++ src/u4.cpp	2008-07-23 11:29:24.860534000 +0100
@@ -33,6 +33,12 @@
 #include "macosx/osxinit.h"
 #endif
 
+#if defined(__riscos__)
+#include "unixlib/local.h"
+/* Stop unixlib treating the zip files as directories */
+int __feature_imagefs_is_file = 1;
+#endif   
+
 bool verbose = false;
 bool quit = false;
 
