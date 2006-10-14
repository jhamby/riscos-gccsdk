--- glib/gtimer.c.org	2006-10-14 18:49:51.000000000 +0200
+++ glib/gtimer.c	2006-10-14 18:50:14.000000000 +0200
@@ -92,9 +92,7 @@
     {
       initialized = TRUE;
 #if !defined(_POSIX_MONOTONIC_CLOCK) || _POSIX_MONOTONIC_CLOCK >= 0
-      if (sysconf (_SC_MONOTONIC_CLOCK) >= 0)
 	posix_clock = CLOCK_MONOTONIC;
-      else
 #endif
 	posix_clock = CLOCK_REALTIME;
     }
