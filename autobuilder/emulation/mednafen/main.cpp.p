--- src/drivers/main.cpp.orig	2012-03-31 08:55:42.000000000 +0100
+++ src/drivers/main.cpp	2012-03-31 08:57:06.000000000 +0100
@@ -30,6 +30,8 @@
 #include <windows.h>
 #endif

+#include <unixlib/local.h>
+
 #include "main.h"

 #include "input.h"
@@ -581,7 +583,7 @@
 	else
 	#endif
 	{
-         if(!(tmp=MDFNI_LoadGame(path)))
+         if(!(tmp=MDFNI_LoadGame(__unixify(path, 0, NULL, 0, 0))))
 	  return 0;
 	}
 	CurGame = tmp;
