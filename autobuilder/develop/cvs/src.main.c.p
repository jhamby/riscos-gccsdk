--- src/main.c.orig	2011-02-07 03:05:16.823003732 +0100
+++ src/main.c	2011-02-07 03:06:16.433000982 +0100
@@ -24,6 +24,14 @@
 #include "strftime.h"
 #include "xgethostname.h"
 
+#ifdef __riscos__
+#  include "../riscos/rcmd.c"
+#  include "../riscos/ro.c"
+#  include "../riscos/server_if.c"
+#  include <unixlib/local.h>
+int __riscosify_control = __RISCOSIFY_STRICT_UNIX_SPECS | __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_DONT_CHECK_DIR | __RISCOSIFY_FILETYPE_NOT_SET;
+#endif
+
 const char *program_name;
 const char *program_path;
 const char *cvs_cmd_name;
@@ -508,6 +516,13 @@
     int c, err = 0;
     int free_Editor = 0;
 
+#ifdef __riscos__
+    if (getenv ("CVS$FFFEXT"))
+      __riscosify_control |= __RISCOSIFY_FILETYPE_FFF_EXT;
+
+    mkdir("<Wimp$ScrapDir>.cvs", 0777);
+#endif
+
     int help = 0;		/* Has the user asked for help?  This
 				   lets us support the `cvs -H cmd'
 				   convention to give help for cmd. */
