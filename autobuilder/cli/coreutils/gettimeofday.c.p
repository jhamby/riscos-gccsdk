--- lib/gettimeofday.c.orig	2010-06-08 18:30:16.122158847 +0100
+++ lib/gettimeofday.c	2010-06-08 18:31:51.332156263 +0100
@@ -100,7 +100,7 @@
 /* This is a wrapper for gettimeofday.  It is used only on systems
    that lack this function, or whose implementation of this function
    causes problems.  */
-
+#if !(defined __riscos__)
 int
 gettimeofday (struct timeval *restrict tv, void *restrict tz)
 {
@@ -144,3 +144,4 @@
 
 #endif
 }
+#endif
