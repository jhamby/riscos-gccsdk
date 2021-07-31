--- src/rpc-linux.c.orig	2020-05-09 12:56:12.499688844 +0100
+++ src/rpc-linux.c	2020-05-09 12:56:33.175277650 +0100
@@ -24,7 +24,9 @@
 #include <string.h>
 
 #include <pthread.h>
+#ifndef __riscos__
 #include <sys/statvfs.h>
+#endif
 #include <sys/types.h>
 #include <sys/utsname.h>
 #include <sys/wait.h>
@@ -49,6 +49,7 @@
 int
 path_disk_info(const char *path, disk_info *d)
 {
+#ifndef __riscos__
 	struct statvfs s;
 	int ret;
 
@@ -61,7 +62,7 @@
 
 	d->size = (uint64_t) s.f_blocks * (uint64_t) s.f_frsize;
 	d->free = (uint64_t) s.f_bavail * (uint64_t) s.f_frsize;
-
+#endif
 	return 1;
 }
 
