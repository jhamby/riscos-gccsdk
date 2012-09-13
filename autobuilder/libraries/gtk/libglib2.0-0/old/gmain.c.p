--- glib/gmain.c.orig	2011-12-29 15:23:36.000000000 +0000
+++ glib/gmain.c	2011-12-29 15:24:00.000000000 +0000
@@ -2055,10 +2055,10 @@
       {
 	clockid_t best_clockid;
 
-	if (sysconf (_SC_MONOTONIC_CLOCK) >= 0)
+//	if (sysconf (_SC_MONOTONIC_CLOCK) >= 0)
 	  best_clockid = CLOCK_MONOTONIC;
-	else
-	  best_clockid = CLOCK_REALTIME;
+//	else
+//	  best_clockid = CLOCK_REALTIME;
 	g_once_init_leave (&clockid, (gsize)best_clockid);
       }
 #endif
