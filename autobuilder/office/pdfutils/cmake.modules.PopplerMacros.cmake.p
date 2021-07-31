--- cmake/modules/PopplerMacros.cmake.orig	2018-11-25 20:17:01.247472928 +0000
+++ cmake/modules/PopplerMacros.cmake	2018-11-25 20:17:30.354871534 +0000
@@ -100,7 +100,7 @@
 
 if(CMAKE_COMPILER_IS_GNUCXX)
   # set the default compile warnings
-  set(_warn "-Wall -Wextra -Wpedantic")
+  set(_warn "-Wall -Wextra")
   set(_warn "${_warn} -Wno-unused-parameter")
   set(_warn "${_warn} -Wcast-align")
   set(_warn "${_warn} -Wformat-security")
