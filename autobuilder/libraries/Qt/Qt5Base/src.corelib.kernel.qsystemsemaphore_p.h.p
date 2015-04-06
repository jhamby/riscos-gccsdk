--- src/corelib/kernel/qsystemsemaphore_p.h.orig	2013-08-25 19:03:38.000000000 +0100
+++ src/corelib/kernel/qsystemsemaphore_p.h	2013-11-13 19:38:10.914351932 +0000
@@ -53,6 +53,7 @@
 // We mean it.
 //
 
+#include "qplatformdefs.h"
 #include "qsystemsemaphore.h"
 
 #ifndef QT_NO_SYSTEMSEMAPHORE
