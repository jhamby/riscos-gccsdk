--- readom/readom.c.orig	2020-12-29 10:34:18.290593354 +0000
+++ readom/readom.c	2020-12-29 10:35:10.341851127 +0000
@@ -442,6 +442,7 @@
 	/*
 	 * We don't need root privilleges anymore.
 	 */
+#ifndef __riscos__
 #ifdef	HAVE_SETREUID
 	if (setreuid(-1, getuid()) < 0)
 #else
@@ -452,7 +453,7 @@
 #endif
 #endif
 		comerr("Panic cannot set back effective uid.\n");
-
+#endif
 	/* code to use SCG */
 
 	if (scanbus) {
