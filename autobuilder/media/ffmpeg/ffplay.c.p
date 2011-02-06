--- ffplay.c.orig	2011-02-06 12:53:17.000000000 +0000
+++ ffplay.c	2011-02-06 12:54:21.000000000 +0000
@@ -54,6 +54,9 @@
 
 #include <unistd.h>
 #include <assert.h>
+#include "unixlib/local.h"
+
+int __riscosify_control = __RISCOSIFY_NO_PROCESS;
 
 const char program_name[] = "FFplay";
 const int program_birth_year = 2003;
