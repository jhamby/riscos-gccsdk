--- modules/access/file.c.orig	2009-08-09 14:50:57.000000000 -0700
+++ modules/access/file.c	2009-12-15 11:29:27.000000000 -0800
@@ -50,7 +50,7 @@
 #if defined (__linux__)
 #   include <sys/vfs.h>
 #   include <linux/magic.h>
-#elif defined (HAVE_SYS_MOUNT_H)
+#elif defined (HAVE_SYS_MOUNT_H) && !defined(__riscos__)
 #   include <sys/param.h>
 #   include <sys/mount.h>
 #endif
