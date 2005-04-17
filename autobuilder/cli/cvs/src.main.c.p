--- src/main_no_riscos.c	2005-04-17 20:52:58.000000000 +0200
+++ src/main.c	2005-04-17 20:58:03.000000000 +0200
@@ -21,6 +21,12 @@
 extern int gethostname (char *, int);
 #endif
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+
+int __riscosify_control = __RISCOSIFY_STRICT_UNIX_SPECS | __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_DONT_CHECK_DIR | __RISCOSIFY_FILETYPE_NOT_SET;
+#endif
+
 const char *program_name;
 const char *program_path;
 const char *cvs_cmd_name;
@@ -427,6 +433,11 @@
     int free_Editor = 0;
     int free_Tmpdir = 0;
 
+#ifdef __riscos__
+    if (getenv("CVS$FFFEXT"))
+      __riscosify_control |= __RISCOSIFY_FILETYPE_FFF_EXT;
+#endif
+
     int help = 0;		/* Has the user asked for help?  This
 				   lets us support the `cvs -H cmd'
 				   convention to give help for cmd. */
