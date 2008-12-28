--- subversion/svn/main.c.orig	2008-12-27 22:10:31.000000000 -0800
+++ subversion/svn/main.c	2008-12-27 22:11:01.000000000 -0800
@@ -49,6 +49,15 @@
 
 #include "svn_private_config.h"
 
+#ifdef __riscos__
+
+#include <unixlib/local.h>
+
+int __riscosify_control = __RISCOSIFY_STRICT_UNIX_SPECS;
+int __feature_imagefs_is_file = 1;
+
+#endif
+
 
 /*** Option Processing ***/
 
