--- src/main/main.cpp.orig	2016-02-21 10:50:12.214833732 +0000
+++ src/main/main.cpp	2016-02-21 10:50:57.446833778 +0000
@@ -22,6 +22,11 @@
 #include <QMessageBox> // For QT_REQUIRE_VERSION
 #include <iostream>
 
+#ifdef __riscos__
+#include <features.h>
+int __stack_size __attribute__ ((visibility ("default"))) = 1024 * 1024; // 1MB stack
+#endif
+
 #if defined(Q_OS_LINUX) || defined(__GLIBC__) || defined(__FreeBSD__) || defined(__HAIKU__)
 #include <signal.h>
 #include <execinfo.h>
