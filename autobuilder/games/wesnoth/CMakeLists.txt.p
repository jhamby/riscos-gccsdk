--- CMakeLists.txt.orig	2011-04-06 13:48:23.306290200 +0100
+++ CMakeLists.txt	2011-04-20 12:45:31.761315500 +0100
@@ -18,6 +18,8 @@
 
 set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
 
+set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
+
 # use our own version of FindBoost.cmake and other Find* scripts
 set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
 
