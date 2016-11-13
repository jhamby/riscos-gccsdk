--- ./libs/filesystem/v3/src/operations.cpp.orig	2012-01-28 14:40:16.000000000 +0000
+++ ./libs/filesystem/v3/src/operations.cpp	2016-11-13 12:05:03.101648265 +0000
@@ -81,7 +81,7 @@
     const fs::path dot_path(".");
     const fs::path dot_dot_path("..");
 #   include <sys/types.h>
-#   if !defined(__APPLE__) && !defined(__OpenBSD__)
+#   if !defined(__APPLE__) && !defined(__OpenBSD__) && !defined(__riscos__)
 #     include <sys/statvfs.h>
 #     define BOOST_STATVFS statvfs
 #     define BOOST_STATVFS_F_FRSIZE vfs.f_frsize
@@ -89,6 +89,9 @@
 #     ifdef __OpenBSD__
 #     include <sys/param.h>
 #     endif
+#ifdef __riscos__
+#     include <sys/statfs.h>
+#endif
 #     include <sys/mount.h>
 #     define BOOST_STATVFS statfs
 #     define BOOST_STATVFS_F_FRSIZE static_cast<boost::uintmax_t>(vfs.f_bsize)
