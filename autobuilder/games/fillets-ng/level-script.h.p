--- src/level/level-script.h.orig	2012-09-20 13:05:31.000000000 +0100
+++ src/level/level-script.h	2012-09-20 13:06:32.000000000 +0100
@@ -2,7 +2,7 @@
 #define HEADER_LEVEL_SCRIPT_H

 extern "C" {
-#include "lua.h"
+#include "lua50/lua.h"
 }

 extern int script_level_save(lua_State *L) throw();
