--- zzip/mmapped.c.orig	2016-11-13 10:48:40.709643581 +0000
+++ zzip/mmapped.c	2016-11-13 10:54:52.513643961 +0000
@@ -28,7 +28,7 @@
 
 #define _ZZIP_DISK_FILE_STRUCT 1
 
-#ifdef __linux__
+#if defined(__linux__) || defined(__riscos__)
 #define _GNU_SOURCE _glibc_developers_are_idiots_to_call_strndup_gnu_specific_
 #endif
 
@@ -39,6 +39,7 @@
 #include <zzip/__fnmatch.h>
 
 #include <stdlib.h>
+#include <string.h>
 #include <sys/stat.h>
 
 #if   defined ZZIP_HAVE_UNISTD_H
