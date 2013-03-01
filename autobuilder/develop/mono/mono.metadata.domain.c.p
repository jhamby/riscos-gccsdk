--- mono/metadata/domain.c.orig	2013-01-08 18:41:07.000000000 +0000
+++ mono/metadata/domain.c	2013-01-16 19:41:47.000000000 +0000
@@ -39,6 +39,11 @@
 #include <metadata/profiler-private.h>
 #include <mono/metadata/coree.h>
 
+#ifdef __riscos__
+#include <unixlib/local.h>
+#include <sys/param.h>
+#endif
+
 /* #define DEBUG_DOMAIN_UNLOAD */
 
 /* we need to use both the Tls* functions and __thread because
@@ -1308,6 +1313,12 @@
 	
 	/* Get a list of runtimes supported by the exe */
 	if (exe_filename != NULL) {
+#ifdef __riscos__
+		char ux_exe_filename [MAXPATHLEN];
+
+		if (__unixify_std (exe_filename, ux_exe_filename, MAXPATHLEN, 0) != NULL)
+			exe_filename = ux_exe_filename;
+#endif
 		/*
 		 * This function will load the exe file as a MonoImage. We need to close it, but
 		 * that would mean it would be reloaded later. So instead, we save it to
