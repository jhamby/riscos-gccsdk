--- libs/filesystem/src/operations.cpp.orig	2009-10-13 14:50:45.000000000 +0100
+++ libs/filesystem/src/operations.cpp	2011-02-16 09:43:37.198664200 +0000
@@ -58,7 +58,7 @@
 
 # else // BOOST_POSIX_API
 #   include <sys/types.h>
-#   if !defined(__APPLE__) && !defined(__OpenBSD__)
+#   if !defined(__APPLE__) && !defined(__OpenBSD__) && !defined(__riscos__)
 #     include <sys/statvfs.h>
 #     define BOOST_STATVFS statvfs
 #     define BOOST_STATVFS_F_FRSIZE vfs.f_frsize
@@ -66,6 +66,9 @@
 #ifdef __OpenBSD__
 #     include <sys/param.h>
 #endif
+#ifdef __riscos__
+#     include <sys/statfs.h>
+#endif
 #     include <sys/mount.h>
 #     define BOOST_STATVFS statfs
 #     define BOOST_STATVFS_F_FRSIZE static_cast<boost::uintmax_t>( vfs.f_bsize )
