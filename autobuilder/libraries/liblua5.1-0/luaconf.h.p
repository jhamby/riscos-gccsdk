--- src/luaconf.h.orig	2021-03-06 14:21:03.571954528 +0000
+++ src/luaconf.h	2021-03-06 14:23:28.234698036 +0000
@@ -95,13 +95,13 @@
 
 #else
 /* This defines DEB_HOST_MULTIARCH */
-#include "lua5.1-deb-multiarch.h"
+//#include "lua5.1-deb-multiarch.h"
 #define LUA_ROOT	"/usr/local/"
 #define LUA_ROOT2	"/usr/"
 #define LUA_LDIR	LUA_ROOT "share/lua/5.1/"
 #define LUA_LDIR2	LUA_ROOT2 "share/lua/5.1/"
 #define LUA_CDIR	LUA_ROOT "lib/lua/5.1/"
-#define LUA_CDIR2	LUA_ROOT2 "lib/" DEB_HOST_MULTIARCH "/lua/5.1/"
+#define LUA_CDIR2	LUA_ROOT2 "lib/" "/lua/5.1/"
 #define LUA_CDIR3	LUA_ROOT2 "lib/lua/5.1/"
 #define LUA_PATH_DEFAULT  \
 		"./?.lua;"  LUA_LDIR"?.lua;"  LUA_LDIR"?/init.lua;" \
