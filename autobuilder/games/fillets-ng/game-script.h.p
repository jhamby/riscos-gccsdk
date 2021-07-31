--- src/level/game-script.h.orig	2012-09-20 13:05:21.000000000 +0100
+++ src/level/game-script.h	2012-09-20 13:05:53.000000000 +0100
@@ -2,7 +2,7 @@
 #define HEADER_GAME_SCRIPT_H

 extern "C" {
-#include "lua.h"
+#include "lua50/lua.h"
 }

 extern int script_game_setRoomWaves(lua_State *L) throw();
