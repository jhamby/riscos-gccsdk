--- src/misc.c.orig	2018-04-30 05:45:06.000000000 +1200
+++ src/misc.c	2020-10-23 21:50:45.741781705 +1300
@@ -25,6 +25,9 @@
 #include <unlinkdir.h>
 #include <utimens.h>
 
+#include <unixlib/local.h>
+int __riscosify_control = __RISCOSIFY_FILETYPE_EXT | __RISCOSIFY_FILETYPE_FFF_EXT;
+#define FORCE_XYZ 
 #ifndef DOUBLE_SLASH_IS_DISTINCT_ROOT
 # define DOUBLE_SLASH_IS_DISTINCT_ROOT 0
 #endif
