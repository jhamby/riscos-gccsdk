--- src/plan/dialog-script.h.orig	2012-09-20 13:00:48.000000000 +0100
+++ src/plan/dialog-script.h	2012-09-20 13:00:59.000000000 +0100
@@ -2,7 +2,7 @@
 #define HEADER_DIALOG_SCRIPT_H

 extern "C" {
-#include "lua.h"
+#include "lua50/lua.h"
 }

 extern int script_game_planAction(lua_State *L) throw();
