--- lib/spawni.c.orig	2021-03-27 10:25:20.461247064 +0000
+++ lib/spawni.c	2021-03-27 10:28:07.378487309 +0000
@@ -29,7 +29,7 @@
 # define O_LARGEFILE 0
 #endif
 
-#if _LIBC || HAVE_PATHS_H
+#if (_LIBC || HAVE_PATHS_H) && !defined(__riscos__)
 # include <paths.h>
 #else
 # define _PATH_BSHELL BOURNE_SHELL
