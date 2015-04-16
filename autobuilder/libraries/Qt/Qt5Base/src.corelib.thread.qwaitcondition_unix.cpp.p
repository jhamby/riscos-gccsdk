--- src/corelib/thread/qwaitcondition_unix.cpp.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/thread/qwaitcondition_unix.cpp	2015-04-12 13:43:51.552657030 +0100
@@ -70,7 +70,7 @@
     pthread_condattr_t condattr;
 
     pthread_condattr_init(&condattr);
-#if !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID) && (_POSIX_MONOTONIC_CLOCK-0 >= 0)
+#if !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID) && !defined(Q_OS_RISCOS) && (_POSIX_MONOTONIC_CLOCK-0 >= 0)
     if (QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
         pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
 #endif
