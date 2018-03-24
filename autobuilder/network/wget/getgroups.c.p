--- lib/getgroups.c.orig	2018-03-21 21:55:07.232255000 +1300
+++ lib/getgroups.c	2018-03-21 23:32:16.441165989 +1300
@@ -19,10 +19,15 @@
 
 #include <config.h>
 
+#ifndef __riscos__
 #include <unistd.h>
+#include <stdlib.h>
+#endif
+#ifdef __riscos__
+#include <sys/select.h>
+#endif
 
 #include <errno.h>
-#include <stdlib.h>
 #include <stdint.h>
 
 #if !HAVE_GETGROUPS
