--- src/gengine/ScriptState.cpp.orig	2012-09-20 12:58:44.000000000 +0100
+++ src/gengine/ScriptState.cpp	2012-09-20 12:59:01.000000000 +0100
@@ -13,8 +13,8 @@
 #include "ScriptException.h"

 extern "C" {
-#include "lualib.h"
-#include "lauxlib.h"
+#include "lua50/lualib.h"
+#include "lua50/lauxlib.h"
 }

 #include "def-script.h"
