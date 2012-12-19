--- src/gengine/ScriptAgent.h.orig	2012-09-20 12:50:58.000000000 +0100
+++ src/gengine/ScriptAgent.h	2012-09-20 12:51:12.000000000 +0100
@@ -11,7 +11,7 @@
 #include <string>

 extern "C" {
-#include "lua.h"
+#include "lua50/lua.h"
 }

 /**
