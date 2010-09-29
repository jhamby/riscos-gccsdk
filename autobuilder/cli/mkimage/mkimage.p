--- tools/mkimage.c.orig	2010-09-29 20:11:37.000000000 +0100
+++ tools/mkimage.c	2010-09-29 20:11:37.000000000 +0100
@@ -429,7 +429,8 @@
 #if defined(_POSIX_SYNCHRONIZED_IO) && \
    !defined(__sun__) && \
    !defined(__FreeBSD__) && \
-   !defined(__APPLE__)
+   !defined(__APPLE__) && \
+   !defined(__riscos__)
 	(void) fdatasync (ifd);
 #else
 	(void) fsync (ifd);
@@ -472,7 +473,8 @@
 #if defined(_POSIX_SYNCHRONIZED_IO) && \
    !defined(__sun__) && \
    !defined(__FreeBSD__) && \
-   !defined(__APPLE__)
+   !defined(__APPLE__) && \
+   !defined(__riscos__)
 	(void) fdatasync (ifd);
 #else
 	(void) fsync (ifd);
