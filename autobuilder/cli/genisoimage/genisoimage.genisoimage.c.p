--- ./genisoimage/genisoimage.c.orig	2020-12-09 18:37:14.061219055 +0000
+++ ./genisoimage/genisoimage.c	2020-12-09 18:38:19.211931011 +0000
@@ -2660,6 +2660,7 @@
 		}
 	}
 	/* We don't need root privilleges anymore. */
+#ifndef __riscos__
 #ifdef	HAVE_SETREUID
 	if (setreuid(-1, getuid()) < 0)
 #else
@@ -2677,7 +2678,7 @@
 		exit(1);
 	}
 #endif
-
+#endif
 
 #ifdef	no_more_needed
 #ifdef __NetBSD__
