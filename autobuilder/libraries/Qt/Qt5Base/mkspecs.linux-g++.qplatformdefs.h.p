--- mkspecs/linux-g++/qplatformdefs.h.orig	2014-05-15 18:12:18.000000000 +0100
+++ mkspecs/linux-g++/qplatformdefs.h	2014-05-28 20:35:25.335739715 +0100
@@ -42,6 +42,13 @@
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
@@ -97,4 +104,5 @@
 #define QT_VSNPRINTF            ::vsnprintf
 #endif
 
+#endif // __riscos__
 #endif // QPLATFORMDEFS_H
