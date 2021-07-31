--- src/git.c.orig	2019-05-10 22:14:33.939273977 +0100
+++ src/git.c	2019-05-10 22:15:45.346233157 +0100
@@ -105,6 +105,13 @@
 
 #endif
 
+#if defined(__riscos)
+#include <unixlib/local.h>
+#include <features.h>
+int __riscosify_control = __RISCOSIFY_FILETYPE_EXT;
+int __feature_imagefs_is_file = 1;
+#endif
+
 /**
  * Main entry.
  *
