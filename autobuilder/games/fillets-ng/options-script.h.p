--- src/gengine/options-script.h.orig	2012-09-20 12:53:46.000000000 +0100
+++ src/gengine/options-script.h	2012-09-20 12:54:32.000000000 +0100
@@ -2,7 +2,7 @@
 #define HEADER_OPTIONS_SCRIPT_H

 extern "C" {
-#include "lua.h"
+#include "lua50/lua.h"
 }

 extern int script_options_sendMsg(lua_State *L) throw();
