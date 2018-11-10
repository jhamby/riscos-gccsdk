--- src/sysdir.c.orig	2018-11-10 11:20:57.033832763 +0000
+++ src/sysdir.c	2018-11-10 11:21:43.945341176 +0000
@@ -47,7 +47,9 @@
 
 	assert(out);
 
+#ifndef GIT_RISCOS
 	if ((buflen = sysconf(_SC_GETPW_R_SIZE_MAX)) == -1)
+#endif
 		buflen = 1024;
 
 	do {
