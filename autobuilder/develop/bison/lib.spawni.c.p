--- lib/spawni.c.orig	2021-11-06 15:14:55.590174238 +0000
+++ lib/spawni.c	2021-11-06 15:18:47.261178680 +0000
@@ -31,6 +31,9 @@
 
 #if _LIBC || HAVE_PATHS_H
 # include <paths.h>
+#if defined(__riscos__)
+# define _PATH_BSHELL "bash"
+#endif
 #else
 # define _PATH_BSHELL BOURNE_SHELL
 #endif
