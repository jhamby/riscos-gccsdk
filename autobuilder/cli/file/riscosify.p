--- src/file.c.orig	2003-05-18 22:06:21.000000000 +0100
+++ src/file.c	2003-05-18 21:06:22.000000000 +0100
@@ -73,6 +73,10 @@
 
 #include <netinet/in.h>		/* for byte swapping */
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+#endif
+
 #include "patchlevel.h"
 
 #ifndef	lint
@@ -165,6 +169,7 @@
 	};
 #endif
 
+
 #ifdef LC_CTYPE
 	setlocale(LC_CTYPE, ""); /* makes islower etc work for other langs */
 #endif
@@ -295,6 +300,11 @@
 		break;
 	}
 
+#ifdef __riscos__
+       __riscosify_control = __RISCOSIFY_NO_PROCESS;
+       __feature_imagefs_is_file = 1;
+#endif
+
 	if (optind == argc) {
 		if (!didsomefiles) {
 			usage();
