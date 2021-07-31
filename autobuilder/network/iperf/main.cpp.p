--- src/main.cpp.orig	2021-01-19 17:19:29.814336066 +0000
+++ src/main.cpp	2021-01-19 17:20:07.637604614 +0000
@@ -194,6 +194,7 @@
 		fprintf(stderr, "Iperf client cannot be run as a daemon\n");
 		return 0;
 	    }
+#ifndef __riscos__
 	    if (daemon(1, 1) < 0) {
 	        perror("daemon");
 	    }
@@ -202,6 +203,7 @@
 	    fclose(stdout);
 	    fclose(stderr);
 	    fclose(stdin);
+#endif
 	}
 #endif
         // initialize client(s)
