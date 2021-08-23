--- ./src/corelib/kernel/qtimerinfo_unix.cpp.orig	2020-10-27 01:02:11.000000000 -0700
+++ ./src/corelib/kernel/qtimerinfo_unix.cpp	2021-08-22 14:45:27.425912561 -0700
@@ -212,10 +212,10 @@
 }
 
 #ifdef QTIMERINFO_DEBUG
-QDebug operator<<(QDebug s, timeval tv)
+QDebug operator<<(QDebug s, timespec tv)
 {
     QDebugStateSaver saver(s);
-    s.nospace() << tv.tv_sec << "." << qSetFieldWidth(6) << qSetPadChar(QChar(48)) << tv.tv_usec << Qt::reset;
+    s.nospace() << tv.tv_sec << "." << qSetFieldWidth(9) << qSetPadChar(QChar(48)) << tv.tv_nsec << Qt::reset;
     return s;
 }
 QDebug operator<<(QDebug s, Qt::TimerType t)
@@ -611,11 +611,11 @@
         float diff;
         if (currentTime < currentTimerInfo->expected) {
             // early
-            timeval early = currentTimerInfo->expected - currentTime;
-            diff = -(early.tv_sec + early.tv_usec / 1000000.0);
+            timespec early = currentTimerInfo->expected - currentTime;
+            diff = -(early.tv_sec + early.tv_nsec / 1000000000.0);
         } else {
-            timeval late = currentTime - currentTimerInfo->expected;
-            diff = late.tv_sec + late.tv_usec / 1000000.0;
+            timespec late = currentTime - currentTimerInfo->expected;
+            diff = late.tv_sec + late.tv_nsec / 1000000000.0;
         }
         currentTimerInfo->cumulativeError += diff;
         ++currentTimerInfo->count;
