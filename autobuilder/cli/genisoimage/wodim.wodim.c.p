--- ./wodim/wodim.c.orig	2020-12-09 18:38:51.303296546 +0000
+++ ./wodim/wodim.c	2020-12-09 18:39:54.822040692 +0000
@@ -458,6 +458,7 @@
 	 *
 	 * XXX Below this point we do not need root privilleges anymore.
 	 */
+#ifndef __riscos__
 	if (geteuid() != getuid()) {	/* AIX does not like to do this */
 					/* If we are not root		*/
 #ifdef	HAVE_SETREUID
@@ -471,6 +472,7 @@
 #endif
 			comerr("Panic cannot set back effective uid.\n");
 	}
+#endif
 
 #ifdef __linux__
 	/* get the rawio capability */
