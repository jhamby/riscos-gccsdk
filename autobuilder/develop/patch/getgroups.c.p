--- lib/getgroups.c.orig	2021-01-17 18:43:36.481179818 +0000
+++ lib/getgroups.c	2021-01-17 18:46:47.581389927 +0000
@@ -25,6 +25,9 @@
 #include <stdlib.h>
 #include <stdint.h>
 
+#ifdef __riscos__
+#define HAVE_GETGROUPS 1
+#endif
 #if !HAVE_GETGROUPS
 
 /* Provide a stub that fails with ENOSYS, since there is no group
