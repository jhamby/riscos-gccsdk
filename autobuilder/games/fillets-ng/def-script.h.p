--- src/gengine/def-script.h.orig	2012-09-20 12:53:36.000000000 +0100
+++ src/gengine/def-script.h	2012-09-20 12:54:21.000000000 +0100
@@ -8,8 +8,8 @@
 #include <stdexcept>

 extern "C" {
-#include "lua.h"
-#include "lauxlib.h"
+#include "lua50/lua.h"
+#include "lua50/lauxlib.h"
 }


