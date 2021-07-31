--- src/ptimer.c.orig	2018-01-14 23:22:42.000000000 +1300
+++ src/ptimer.c	2018-03-27 12:02:44.432079589 +1300
@@ -121,9 +121,11 @@
     int id;
     int sysconf_name;
   } clocks[] = {
+#ifndef __riscos__
 #if defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK - 0 >= 0
     { CLOCK_MONOTONIC, _SC_MONOTONIC_CLOCK },
 #endif
+#endif
 #ifdef CLOCK_HIGHRES
     { CLOCK_HIGHRES, NO_SYSCONF_CHECK },
 #endif
