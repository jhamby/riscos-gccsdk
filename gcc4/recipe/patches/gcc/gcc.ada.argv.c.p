Index: gcc/ada/argv.c
===================================================================
--- gcc/ada/argv.c	(revision 162881)
+++ gcc/ada/argv.c	(working copy)
@@ -51,6 +51,8 @@
 #include "system.h"
 #endif
 
+#include <string.h>
+
 #include "adaint.h"
 
 /* argc and argv of the main program are saved under gnat_argc and gnat_argv,
