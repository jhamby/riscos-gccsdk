--- genisoimage/CMakeLists.txt.orig	2018-01-07 11:54:13.391670638 +0000
+++ genisoimage/CMakeLists.txt	2018-01-07 11:54:27.231391288 +0000
@@ -6,7 +6,6 @@
 INCLUDE_DIRECTORIES(../include ../libhfs_iso ../wodim ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/include)
 
 INCLUDE(CheckIncludeFiles)
-CHECK_INCLUDE_FILES("magic.h" USE_MAGIC)
 IF(USE_MAGIC)
 	ADD_DEFINITIONS(-DUSE_MAGIC)
 	SET(MAGICLIBS magic)
