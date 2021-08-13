--- Source/ThirdParty/ANGLE/src/compiler/osinclude.h.orig	2015-10-12 21:37:08.000000000 -0700
+++ Source/ThirdParty/ANGLE/src/compiler/osinclude.h	2021-08-13 15:04:01.691752435 -0700
@@ -18,7 +18,7 @@
       defined(__FreeBSD__) || defined(__OpenBSD__) || \
       defined(__sun) || defined(ANDROID) || \
       defined(__GLIBC__) || defined(__GNU__) || \
-      defined(__QNX__)
+      defined(__QNX__) || defined(__riscos__)
 #define ANGLE_OS_POSIX
 #else
 #error Unsupported platform.
