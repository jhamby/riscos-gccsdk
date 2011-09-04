Index: gcc/ada/targext.c
===================================================================
--- gcc/ada/targext.c	(revision 178499)
+++ gcc/ada/targext.c	(working copy)
@@ -41,6 +41,7 @@
 #include "system.h"
 #endif
 #include "coretypes.h"
+#include <stdio.h>
 #include "tm.h"
 
 #ifndef TARGET_OBJECT_SUFFIX
