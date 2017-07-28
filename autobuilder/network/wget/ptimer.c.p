--- src/ptimer.c.orig	2015-05-14 07:28:25.000000000 +1200
+++ src/ptimer.c	2017-07-26 22:13:03.122439987 +1200
@@ -122,9 +122,11 @@
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
