diff -ur subversion-1.5.5.orig/subversion/svn/main.c subversion-1.5.5/subversion/svn/main.c
--- subversion/svn/main.c	2008-12-18 15:08:56.000000000 +0000
+++ subversion/svn/main.c	2009-01-03 20:57:18.000000000 +0000
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
 
@@ -1086,6 +1095,11 @@
   if (svn_cmdline_init("svn", stderr) != EXIT_SUCCESS)
     return EXIT_FAILURE;
 
+#ifdef __riscos__
+  if (getenv("svn$filetypeext"))
+    __riscosify_control |= __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_FILETYPE_NOT_SET;
+#endif
+
   /* Create our top-level pool.  Use a seperate mutexless allocator,
    * given this application is single threaded.
    */
