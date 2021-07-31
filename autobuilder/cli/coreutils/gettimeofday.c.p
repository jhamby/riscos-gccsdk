--- lib/gettimeofday.c.orig	2018-06-27 09:37:37.000000000 +0100
+++ lib/gettimeofday.c	2020-09-05 11:35:14.499639409 +0100
@@ -60,6 +60,7 @@
    gettimeofday function from Mac OS X 10.0.4 (i.e., Darwin 1.3.7) has
    this problem.  */
 
+#ifndef __riscos__
 int
 gettimeofday (struct timeval *restrict tv, void *restrict tz)
 {
@@ -144,3 +145,4 @@
 # endif
 #endif
 }
+#endif
