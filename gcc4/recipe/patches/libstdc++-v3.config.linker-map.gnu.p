--- libstdc++-v3/config/linker-map.gnu.orig	2008-01-17 19:44:43.000000000 +0000
+++ libstdc++-v3/config/linker-map.gnu	2008-01-17 19:43:16.000000000 +0000
@@ -476,6 +476,8 @@
 
   # DO NOT DELETE THIS LINE.  Port-specific symbols, if any, will be here.
 
+    som___got;
+
   local:
     *;
 };
