Index: gcc/ada/cstreams.c
===================================================================
--- gcc/ada/cstreams.c	(revision 176214)
+++ gcc/ada/cstreams.c	(working copy)
@@ -48,6 +48,9 @@
 #include "system.h"
 #endif
 
+#include <string.h>
+#include <unistd.h>
+
 #include "adaint.h"
 
 #ifdef VMS
