--- src/file.c.orig	2008-03-09 17:58:06.000000000 -0700
+++ src/file.c	2008-03-09 17:59:56.000000000 -0700
@@ -68,6 +68,11 @@
 
 #include <netinet/in.h>		/* for byte swapping */
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+int __feature_image_fs_is_file = 1;
+#endif
+
 #include "patchlevel.h"
 
 #ifndef	lint
@@ -326,6 +331,10 @@
 		break;
 	}
 
+#ifdef __riscos__
+       __set_riscosify_control(__RISCOSIFY_NO_PROCESS);
+#endif
+
 	if (optind == argc) {
 		if (!didsomefiles) {
 			usage();
