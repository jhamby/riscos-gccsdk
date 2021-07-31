--- mono/metadata/sgen-stw.c.orig	2013-01-18 16:58:23.000000000 +0000
+++ mono/metadata/sgen-stw.c	2013-01-18 16:59:06.000000000 +0000
@@ -105,6 +105,7 @@
 	int num_threads_died = 0;
 	int sleep_duration = -1;
 
+#ifndef __riscos__
 	for (;;) {
 		int restart_count = 0, restarted_count = 0;
 		/* restart all threads that stopped in the
@@ -171,7 +172,7 @@
 		   again */
 		sgen_wait_for_suspend_ack (restarted_count);
 	}
-
+#endif
 	return num_threads_died;
 }
 
