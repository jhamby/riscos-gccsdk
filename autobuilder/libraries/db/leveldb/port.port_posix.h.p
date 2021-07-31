--- port/port_posix.h.orig	2013-12-10 19:15:01.000000000 +0000
+++ port/port_posix.h	2014-06-16 20:06:49.928243648 +0100
@@ -63,7 +63,7 @@
 #endif
 
 #if defined(OS_MACOSX) || defined(OS_FREEBSD) ||\
-    defined(OS_OPENBSD) || defined(OS_DRAGONFLYBSD)
+    defined(OS_OPENBSD) || defined(OS_DRAGONFLYBSD) || defined(OS_RISCOS)
 // Use fsync() on platforms without fdatasync()
 #define fdatasync fsync
 #endif
