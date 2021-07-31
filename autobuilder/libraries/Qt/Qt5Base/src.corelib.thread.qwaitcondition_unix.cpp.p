--- src/corelib/thread/qwaitcondition_unix.cpp.orig	2015-07-15 18:24:19.830735895 +0100
+++ src/corelib/thread/qwaitcondition_unix.cpp	2015-07-15 18:40:22.114748365 +0100
@@ -77,7 +77,7 @@
 #if defined(Q_OS_ANDROID)
     if (local_condattr_setclock && QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
         local_condattr_setclock(&condattr, CLOCK_MONOTONIC);
-#elif !defined(Q_OS_MAC) && !defined(Q_OS_HAIKU)
+#elif !defined(Q_OS_MAC) && !defined(Q_OS_HAIKU) && !defined(Q_OS_RISCOS)
     if (QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
         pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
 #endif
