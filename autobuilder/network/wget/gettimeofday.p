--- lib/gettimeofday.c.orig	2017-02-05 03:41:52.000000000 +1300
+++ lib/gettimeofday.c	2017-07-26 22:31:56.419330178 +1200
@@ -20,7 +20,12 @@
 #include <config.h>
 
 /* Specification.  */
+#ifndef __riscos__
 #include <sys/time.h>
+#endif
+#ifdef __riscos__
+#include <sys/select.h>
+#endif
 
 #include <time.h>
 
