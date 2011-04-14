Index: gcc/ada/env.c
===================================================================
--- gcc/ada/env.c	(revision 162881)
+++ gcc/ada/env.c	(working copy)
@@ -65,6 +65,9 @@
 #include "system.h"
 #endif /* IN_RTS */
 
+#include <stdio.h>
+#include <stdlib.h>
+
 #if defined (__APPLE__)
 #include <crt_externs.h>
 #endif
