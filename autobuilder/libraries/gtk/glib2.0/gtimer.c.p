--- glib/gtimer.c.org	2006-10-14 18:49:51.000000000 +0200
+++ glib/gtimer.c	2006-10-14 18:50:14.000000000 +0200
@@ -95,10 +95,7 @@
   if (!initialized)
     {
       initialized = TRUE;
-      if (sysconf (_SC_MONOTONIC_CLOCK) >= 0)
 	posix_clock = CLOCK_MONOTONIC;
-      else
-	posix_clock = CLOCK_REALTIME;
     }
 }
 #endif
