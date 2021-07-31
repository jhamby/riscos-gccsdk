--- mkspecs/linux-g++/qplatformdefs.h.orig	2015-02-17 04:56:54.000000000 +0000
+++ mkspecs/linux-g++/qplatformdefs.h	2015-04-12 13:43:51.592657031 +0100
@@ -34,6 +34,13 @@
 #ifndef QPLATFORMDEFS_H
 #define QPLATFORMDEFS_H
 
+#ifdef __riscos__
+// Hack: Despite using the "-xplatform arm-riscos-g++" option during configure,
+// it still includes linux-g++/qplatformdefs.h instead of arm-riscos-g++/
+// qplatformdefs.h.
+#include "../arm-riscos-g++/qplatformdefs.h"
+#else // __riscos__
+
 // Get Qt defines/settings
 
 #include "qglobal.h"
@@ -89,4 +96,5 @@
 #define QT_VSNPRINTF            ::vsnprintf
 #endif
 
+#endif // __riscos__
 #endif // QPLATFORMDEFS_H
