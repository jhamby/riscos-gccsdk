--- Tools/BindHelp/BindHelp.c.orig	2014-05-23 12:09:38.000000000 +0100
+++ Tools/BindHelp/BindHelp.c	2014-05-23 10:51:22.000000000 +0100
@@ -95,7 +95,7 @@
 #include "oslib/osargs.h"
 
 /* From OSLibSupport */
-#include "x.h"
+#include "X.h"
 #include "trace.h"
 
 
@@ -266,6 +266,7 @@
 
 /****************************************************************/
 /* case insensitive strcmp */
+#ifndef __riscos__
 static int stricmp( const char *String1, const char *String2 )
 {
   int iStatus;
@@ -278,7 +279,7 @@
 
   return iStatus;
 }
-
+#endif
 
 /****************************************************************/
 /* compare two strings */
