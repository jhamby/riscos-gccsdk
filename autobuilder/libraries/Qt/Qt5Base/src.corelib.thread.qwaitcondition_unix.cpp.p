--- src/corelib/thread/qwaitcondition_unix.cpp.orig	2013-08-25 19:03:37.000000000 +0100
+++ src/corelib/thread/qwaitcondition_unix.cpp	2013-11-12 19:50:15.792050731 +0000
@@ -78,7 +78,7 @@
     pthread_condattr_t condattr;
 
     pthread_condattr_init(&condattr);
-#if !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID) && (_POSIX_MONOTONIC_CLOCK-0 >= 0)
+#if !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID) && !defined(Q_OS_RISCOS) && (_POSIX_MONOTONIC_CLOCK-0 >= 0)
     if (QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
         pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
 #endif
