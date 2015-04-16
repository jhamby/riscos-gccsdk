--- src/corelib/kernel/qsystemsemaphore_p.h.orig	2015-02-17 04:56:50.000000000 +0000
+++ src/corelib/kernel/qsystemsemaphore_p.h	2015-04-12 13:43:51.640657032 +0100
@@ -45,6 +45,7 @@
 // We mean it.
 //
 
+#include "qplatformdefs.h"
 #include "qsystemsemaphore.h"
 
 #ifndef QT_NO_SYSTEMSEMAPHORE
