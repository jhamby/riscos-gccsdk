--- src/corelib/kernel/qtimerinfo_unix_p.h.orig	2021-08-21 20:21:30.066460371 -0700
+++ src/corelib/kernel/qtimerinfo_unix_p.h	2021-08-21 20:36:54.871317185 -0700
@@ -53,11 +53,11 @@
 
 #include <QtCore/private/qglobal_p.h>
 
-// #define QTIMERINFO_DEBUG
+// #define QTIMERINFO_DEBUG
 
 #include "qabstracteventdispatcher.h"
 
-#include <sys/time.h> // struct timeval
+#include <time.h> // struct timespec
 
 QT_BEGIN_NAMESPACE
 
@@ -71,7 +71,7 @@
     QTimerInfo **activateRef; // - ref from activateTimers
 
 #ifdef QTIMERINFO_DEBUG
-    timeval expected; // when timer is expected to fire
+    timespec expected; // when timer is expected to fire
     float cumulativeError;
     uint count;
 #endif
