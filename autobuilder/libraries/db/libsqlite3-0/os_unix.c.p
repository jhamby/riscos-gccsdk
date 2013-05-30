--- src/os_unix.c.orig	2013-05-30 00:11:25.431940083 +0100
+++ src/os_unix.c	2013-05-30 00:12:30.743647356 +0100
@@ -48,10 +48,12 @@
 
 /* Use posix_fallocate() if it is available
 */
+#ifndef __riscos__
 #if !defined(HAVE_POSIX_FALLOCATE) \
       && (_XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L)
 # define HAVE_POSIX_FALLOCATE 1
 #endif
+#endif
 
 /*
 ** There are various methods for file locking used for concurrency
