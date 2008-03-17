--- src/stat.c.org	2008-03-17 03:54:27.000000000 +0100
+++ src/stat.c	2008-03-17 03:54:31.000000000 +0100
@@ -118,6 +118,7 @@
 #  define STRUCT_STATXFS_F_FSID_IS_INTEGER true
 #  define STATFS_FRSIZE(S) ((S)->block_size)
 # else
+#  include <sys/statfs.h>
 #  define STRUCT_STATVFS struct statfs
 #  define STRUCT_STATXFS_F_FSID_IS_INTEGER STRUCT_STATFS_F_FSID_IS_INTEGER
 #  define STATFS_FRSIZE(S) 0
