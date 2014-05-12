--- gio/glocalfile.c.orig	2014-02-03 17:40:41.000000000 +0000
+++ gio/glocalfile.c	2014-05-08 17:08:34.088137593 +0100
@@ -30,6 +30,10 @@
 #include <unistd.h>
 #endif
 
+#define USE_STATFS
+#undef HAVE_SYS_STATFS_H
+#define HAVE_SYS_STATFS_H 1
+
 #if HAVE_SYS_STATFS_H
 #include <sys/statfs.h>
 #endif
