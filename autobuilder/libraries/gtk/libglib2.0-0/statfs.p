--- gio/glocalfile.c.orig	2008-04-26 18:23:33.000000000 -0700
+++ gio/glocalfile.c	2008-04-26 19:00:01.000000000 -0700
@@ -31,6 +31,10 @@
 #include <unistd.h>
 #endif
 
+#define USE_STATFS
+#undef HAVE_SYS_STATFS_H
+#define HAVE_SYS_STATFS_H 1
+
 #if HAVE_SYS_STATFS_H
 #include <sys/statfs.h>
 #endif
