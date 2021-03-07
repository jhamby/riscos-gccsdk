--- include/unixconf.h.orig	2021-03-07 11:02:32.271502014 +0000
+++ include/unixconf.h	2021-03-07 11:26:41.841511031 +0000
@@ -95,14 +95,14 @@
  *		Ralf Brown, 7/26/89 (from v2.3 hack of 10/10/88)
  */
 
-/* #define NO_FILE_LINKS */	/* if no hard links */
-/* #define LOCKDIR "/usr/games/lib/nethackdir" */	/* where to put locks */
+#define NO_FILE_LINKS	/* if no hard links */
+#define LOCKDIR "/<GLHack$Dir>/locks"	/* where to put locks */
 
 /*
  * If you want the static parts of your playground on a read-only file
  * system, define VAR_PLAYGROUND to be where the variable parts are kept.
  */
-#define VAR_PLAYGROUND "/var/games/glhack"
+#define VAR_PLAYGROUND "/<GLHack$Dir>/var"
 
 
 /*
