--- Source_Files/GameWorld/world.cpp.orig	2002-09-29 05:40:32.000000000 +0100
+++ Source_Files/GameWorld/world.cpp	2008-09-04 11:26:53.578000000 +0100
@@ -52,6 +52,11 @@
 	Inlined the angle normalization; doing it automatically for all the functions that work with angles
 */
 
+#ifdef __riscos__
+/* RISC OS Unixlib assert fails to compile when included later */
+#include "assert.h"
+#endif
+
 #ifndef DEBUG_FAST_CODE
 #undef DEBUG
 // #undef assert(x)
