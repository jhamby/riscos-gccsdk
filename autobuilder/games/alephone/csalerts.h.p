--- Source_Files/CSeries/csalerts.h.orig	2005-06-26 18:50:48.000000000 +0100
+++ Source_Files/CSeries/csalerts.h	2008-09-04 11:26:53.456000000 +0100
@@ -23,6 +23,13 @@
 #ifndef _CSERIES_ALERTS_
 #define _CSERIES_ALERTS_
 
+#ifdef __riscos__
+/* RISC OS assert clashes if defined after redefinition below
+   so include here.
+*/
+#include "assert.h"
+#endif
+
 #if defined(__GNUC__)
 #define NORETURN __attribute__((noreturn))
 #else
