--- lib/gettimeofday.c.orig	2015-01-05 05:46:01.000000000 +1300
+++ lib/gettimeofday.c	2015-03-11 18:43:11.164029091 +1300
@@ -20,7 +20,7 @@
 #include <config.h>
 
 /* Specification.  */
-#include <sys/time.h>
+/*#include <sys/time.h>  ambiguous <sys/time>'s */  
 
 #include <time.h>
 
@@ -97,7 +97,7 @@
 /* This is a wrapper for gettimeofday.  It is used only on systems
    that lack this function, or whose implementation of this function
    causes problems.  */
-
+#if !(defined __riscos__)
 int
 gettimeofday (struct timeval *restrict tv, void *restrict tz)
 {
@@ -152,3 +152,4 @@
 
 #endif
 }
+#endif
