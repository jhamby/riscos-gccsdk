Index: gcc/ada/adaint.c
===================================================================
--- gcc/ada/adaint.c	(revision 177481)
+++ gcc/ada/adaint.c	(working copy)
@@ -108,6 +108,10 @@
 #include "version.h"
 #endif
 
+#include <string.h>
+#include <stdlib.h>
+#include <unistd.h>
+
 #if defined (__MINGW32__)
 
 #if defined (RTX)
