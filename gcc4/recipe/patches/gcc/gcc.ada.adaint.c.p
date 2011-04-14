Index: gcc/ada/adaint.c
===================================================================
--- gcc/ada/adaint.c	(revision 165437)
+++ gcc/ada/adaint.c	(working copy)
@@ -84,6 +84,10 @@
 #include "version.h"
 #endif
 
+#include <string.h>
+#include <stdlib.h>
+#include <unistd.h>
+
 #if defined (__MINGW32__)
 
 #if defined (RTX)
