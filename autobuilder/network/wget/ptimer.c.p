--- src/ptimer.c.orig	2014-02-09 16:00:17.567761530 +0000
+++ src/ptimer.c	2014-02-09 15:58:35.169964444 +0000
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
