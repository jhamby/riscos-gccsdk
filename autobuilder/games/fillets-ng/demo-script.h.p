--- src/state/demo-script.h.orig	2012-09-20 13:03:08.000000000 +0100
+++ src/state/demo-script.h	2012-09-20 13:03:19.000000000 +0100
@@ -2,7 +2,7 @@
 #define HEADER_DEMO_SCRIPT_H

 extern "C" {
-#include "lua.h"
+#include "lua50/lua.h"
 }

 extern int script_demo_display(lua_State *L) throw();
