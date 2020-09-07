--- ./src/uae-cpu/CMakeLists.txt.orig	2020-09-07 08:18:18.501585571 +0100
+++ ./src/uae-cpu/CMakeLists.txt	2020-09-07 08:19:50.287664771 +0100
@@ -7,7 +7,7 @@
 if(CMAKE_CROSSCOMPILING)
 
 	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/build68k
-		COMMAND cc ${CMAKE_CURRENT_SOURCE_DIR}/build68k.c
+		COMMAND /usr/bin/cc ${CMAKE_CURRENT_SOURCE_DIR}/build68k.c
 		           -o ${CMAKE_CURRENT_BINARY_DIR}/build68k
 		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/build68k.c)
 
@@ -17,7 +17,7 @@
 			${CMAKE_CURRENT_BINARY_DIR}/build68k)
 
 	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gencpu
-		COMMAND cc -I${CMAKE_CURRENT_SOURCE_DIR}
+		COMMAND /usr/bin/cc -I${CMAKE_CURRENT_SOURCE_DIR}
 			   cpudefs.c ${CMAKE_CURRENT_SOURCE_DIR}/gencpu.c
 			   ${CMAKE_CURRENT_SOURCE_DIR}/readcpu.c
 			   -o ${CMAKE_CURRENT_BINARY_DIR}/gencpu
