Index: gcc/ada/cstreams.c
===================================================================
--- gcc/ada/cstreams.c	(revision 162881)
+++ gcc/ada/cstreams.c	(working copy)
@@ -44,6 +44,9 @@
 #include "system.h"
 #endif
 
+#include <string.h>
+#include <unistd.h>
+
 #include "adaint.h"
 
 #ifdef VMS
