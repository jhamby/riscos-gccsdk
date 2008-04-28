--- gcc/ada/raise.c.orig	2008-04-27 20:40:15.000000000 +0200
+++ gcc/ada/raise.c	2008-04-27 20:45:45.000000000 +0200
@@ -39,6 +39,8 @@
 #ifdef IN_RTS
 #include "tconfig.h"
 #include "tsystem.h"
+
+#include <time.h>
 #else
 #include "config.h"
 #include "system.h"
