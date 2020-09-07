--- ./src/cpu/CMakeLists.txt.orig	2020-09-07 08:18:06.297844972 +0100
+++ ./src/cpu/CMakeLists.txt	2020-09-07 08:19:00.336702593 +0100
@@ -19,7 +19,7 @@
 if(CMAKE_CROSSCOMPILING)
 
 	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/build68k
-		COMMAND cc -I${CMAKE_CURRENT_SOURCE_DIR}
+		COMMAND /usr/bin/cc -I${CMAKE_CURRENT_SOURCE_DIR}
 			   ${CMAKE_CURRENT_SOURCE_DIR}/build68k.c
 			   ${CMAKE_CURRENT_SOURCE_DIR}/writelog.c
 			   -o ${CMAKE_CURRENT_BINARY_DIR}/build68k
@@ -31,7 +31,7 @@
 			${CMAKE_CURRENT_BINARY_DIR}/build68k)
 
 	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gencpu
-		COMMAND cc -I${CMAKE_CURRENT_SOURCE_DIR}
+		COMMAND /usr/bin/cc -I${CMAKE_CURRENT_SOURCE_DIR}
 			   cpudefs.c ${CMAKE_CURRENT_SOURCE_DIR}/gencpu.c
 			   ${CMAKE_CURRENT_SOURCE_DIR}/readcpu.c
 			   -o ${CMAKE_CURRENT_BINARY_DIR}/gencpu
