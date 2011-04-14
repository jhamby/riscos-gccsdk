Index: gcc/ada/raise-gcc.c
===================================================================
--- gcc/ada/raise-gcc.c	(revision 162881)
+++ gcc/ada/raise-gcc.c	(working copy)
@@ -41,6 +41,7 @@
    we include it.  */
 #ifndef FIRST_PSEUDO_REGISTER
 #include "coretypes.h"
+#include <stdio.h>
 #include "tm.h"
 #endif
 #include <sys/stat.h>
