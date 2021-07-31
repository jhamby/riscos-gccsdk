--- gthread/gthread-posix.c.orig	2007-12-29 09:56:47.000000000 -0800
+++ gthread/gthread-posix.c	2007-12-29 09:58:34.000000000 -0800
@@ -148,10 +148,10 @@
 #endif /* HAVE_PRIORITIES */
 
 #ifdef USE_CLOCK_GETTIME
- if (sysconf (_SC_MONOTONIC_CLOCK) >= 0)
+ //if (sysconf (_SC_MONOTONIC_CLOCK) >= 0)
    posix_clock = CLOCK_MONOTONIC;
- else
-   posix_clock = CLOCK_REALTIME;
+ //else
+ //  posix_clock = CLOCK_REALTIME;
 #endif
 }
 #endif /* _SC_THREAD_STACK_MIN || HAVE_PRIORITIES */
