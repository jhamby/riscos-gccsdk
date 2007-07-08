--- libstdc++-v3/config/linker-map.gnu.orig	2007-07-08 16:03:12.000000000 +0100
+++ libstdc++-v3/config/linker-map.gnu	2007-07-08 16:00:42.000000000 +0100
@@ -476,6 +476,8 @@
 
   # DO NOT DELETE THIS LINE.  Port-specific symbols, if any, will be here.
 
+    __som_got__;
+
   local:
     *;
 };
