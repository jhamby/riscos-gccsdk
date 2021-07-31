--- src/copy.c.orig	2020-02-25 12:46:39.163632282 +0000
+++ src/copy.c	2020-02-25 12:49:05.440673107 +0000
@@ -97,7 +97,7 @@
 #endif
 
 #ifndef HAVE_MKFIFO
-static int
+int
 rpl_mkfifo (char const *file, mode_t mode)
 {
   errno = ENOTSUP;
