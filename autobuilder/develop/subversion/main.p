--- subversion/svn/main.c.orig	2009-03-20 15:49:12.000000000 +0100
+++ subversion/svn/main.c	2009-08-11 23:18:18.000000000 +0200
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
 
@@ -1108,6 +1117,11 @@
   if (svn_cmdline_init("svn", stderr) != EXIT_SUCCESS)
     return EXIT_FAILURE;
 
+#ifdef __riscos__
+  if (getenv("svn$filetypeext"))
+    __riscosify_control |= __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_FILETYPE_NOT_SET;
+#endif
+
   /* Create our top-level pool.  Use a separate mutexless allocator,
    * given this application is single threaded.
    */
