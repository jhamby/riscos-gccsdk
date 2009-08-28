--- modules/access/file.c.orig	2009-08-28 11:29:00.000000000 -0700
+++ modules/access/file.c	2009-08-28 11:30:51.000000000 -0700
@@ -51,7 +51,7 @@
 #   include <sys/vfs.h>
 #   include <linux/magic.h>
 #   define HAVE_FSTATFS 1
-#elif defined (HAVE_SYS_MOUNT_H)
+#elif defined (HAVE_SYS_MOUNT_H) && !defined(__riscos__)
 #   include <sys/mount.h>
 #   define HAVE_FSTATFS 1
 #endif
