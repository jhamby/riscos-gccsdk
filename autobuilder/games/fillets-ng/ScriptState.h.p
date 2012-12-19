--- src/gengine/ScriptState.h.orig	2012-09-20 12:54:05.000000000 +0100
+++ src/gengine/ScriptState.h	2012-09-20 12:54:44.000000000 +0100
@@ -9,7 +9,7 @@
 #include <string>

 extern "C" {
-#include "lua.h"
+#include "lua50/lua.h"
 }

 /**
