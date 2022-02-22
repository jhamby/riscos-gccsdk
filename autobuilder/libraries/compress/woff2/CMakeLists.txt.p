--- CMakeLists.txt.orig2	2020-12-24 21:08:50.000000000 +0000
+++ CMakeLists.txt	2020-12-24 21:12:35.094962016 +0000
@@ -88,7 +88,7 @@
 
 foreach(lib woff2common woff2dec woff2enc)
   set_target_properties(${lib} PROPERTIES
-    SOVERSION ${WOFF2_VERSION}
+    SOVERSION 1
     VERSION ${WOFF2_VERSION}
     POSITION_INDEPENDENT_CODE TRUE)
 endforeach()
