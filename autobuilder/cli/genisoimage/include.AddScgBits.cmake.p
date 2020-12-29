--- include/AddScgBits.cmake.orig	2006-11-23 18:57:28.000000000 +0000
+++ include/AddScgBits.cmake	2020-12-09 17:51:37.303027941 +0000
@@ -6,7 +6,6 @@
 LIST(APPEND EXTRA_LIBS "usal")
 
 INCLUDE(CheckIncludeFiles)
-CHECK_INCLUDE_FILES("stdio.h;camlib.h" HAVE_CAMLIB_H)
 
 IF(HAVE_CAMLIB_H)
 
@@ -55,10 +54,9 @@
 
 
 SET(CMAKE_REQUIRED_LIBRARIES )
-   CHECK_C_SOURCE_COMPILES("${TESTSRC}" LIBC_SCHED)
 
 IF(NOT LIBC_SCHED)
-   LIST(APPEND EXTRA_LIBS -lrt)
+#   LIST(APPEND EXTRA_LIBS -lrt)
    #MESSAGE("Using librt for realtime functions")
 ENDIF(NOT LIBC_SCHED)
 
