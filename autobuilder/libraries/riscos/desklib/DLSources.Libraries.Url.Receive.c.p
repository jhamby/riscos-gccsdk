--- !DLSources/Libraries/Url/Receive.c.orig	2023-08-11 17:14:03.116475879 -0700
+++ !DLSources/Libraries/Url/Receive.c	2023-08-11 17:14:53.232369590 -0700
@@ -17,7 +17,7 @@
 #include "DeskLib:WimpSWIs.h"
 #include "DeskLib:Url.h"
     
-#include "Url.h"
+#include "UrlDefs.h"
 
 /*
  * These are used to keep a simple linked list of
