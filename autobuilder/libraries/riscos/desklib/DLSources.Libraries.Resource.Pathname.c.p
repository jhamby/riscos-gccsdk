--- !DLSources/Libraries/Resource/Pathname.c.orig	2023-08-11 16:34:02.397329555 -0700
+++ !DLSources/Libraries/Resource/Pathname.c	2023-08-11 16:50:22.011716909 -0700
@@ -17,4 +17,4 @@
 #include "Resource.h"
 
 
-char resource_pathname[32] = "\0";      /* no path (use CSD) if user not set */
+char resource_pathname[60] = "\0";      /* no path (use CSD) if user not set */
