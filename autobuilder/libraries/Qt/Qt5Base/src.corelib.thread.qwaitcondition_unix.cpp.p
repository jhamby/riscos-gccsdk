--- ./src/corelib/thread/qwaitcondition_unix.cpp.orig	2021-08-14 22:19:58.468129377 -0700
+++ ./src/corelib/thread/qwaitcondition_unix.cpp	2021-08-14 22:44:02.196569500 -0700
@@ -77,7 +77,7 @@
 #if defined(Q_OS_ANDROID)
     if (local_condattr_setclock && QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
         local_condattr_setclock(&condattr, CLOCK_MONOTONIC);
-#elif !defined(Q_OS_MAC)
+#elif !defined(Q_OS_MAC) && !defined(Q_OS_RISCOS)
     if (QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
         pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
 #endif
