--- src/resolver.lua.old	2003-02-26 20:35:20.000000000 +0000
+++ src/resolver.lua	2003-04-05 20:00:26.000000000 +0100
@@ -2,7 +2,7 @@
 
 colloquy = {}
 
-dofile("config.lua")
+dofile("<Colloquy$Dir>/config.lua")
 
 server = bind(colloquy.resolverIP or "0.0.0.0", colloquy.resolverPort or 1235);
 
