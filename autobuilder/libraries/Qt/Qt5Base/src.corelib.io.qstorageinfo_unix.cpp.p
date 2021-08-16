--- ./src/corelib/io/qstorageinfo_unix.cpp.orig	2017-09-06 05:13:54.000000000 -0700
+++ ./src/corelib/io/qstorageinfo_unix.cpp	2021-08-14 22:41:03.763073402 -0700
@@ -62,6 +62,8 @@
 #  include <VolumeRoster.h>
 #  include <fs_info.h>
 #  include <sys/statvfs.h>
+#elif defined(Q_OS_RISCOS)
+#  include <sys/vfs.h>
 #else
 #  include <sys/statvfs.h>
 #endif
@@ -90,6 +92,9 @@
 #elif defined(Q_OS_HAIKU)
 #  define QT_STATFSBUF struct statvfs
 #  define QT_STATFS    ::statvfs
+#elif defined(Q_OS_RISCOS)
+#  define QT_STATFS    ::statfs
+#  define QT_STATFSBUF struct statfs
 #else
 #  if defined(QT_LARGEFILE_SUPPORT)
 #    define QT_STATFSBUF struct statvfs64
@@ -114,7 +119,7 @@
     inline QByteArray fileSystemType() const;
     inline QByteArray device() const;
 private:
-#if defined(Q_OS_BSD4)
+#if defined(Q_OS_BSD4) || defined(Q_OS_RISCOS)
     QT_STATFSBUF *stat_buf;
     int entryCount;
     int currentIndex;
@@ -522,7 +527,7 @@
         valid = true;
         ready = true;
 
-#if defined(Q_OS_BSD4) && !defined(Q_OS_NETBSD)
+#if (defined(Q_OS_BSD4) && !defined(Q_OS_NETBSD)) || defined(Q_OS_RISCOS)
         bytesTotal = statfs_buf.f_blocks * statfs_buf.f_bsize;
         bytesFree = statfs_buf.f_bfree * statfs_buf.f_bsize;
         bytesAvailable = statfs_buf.f_bavail * statfs_buf.f_bsize;
@@ -532,6 +537,9 @@
         bytesAvailable = statfs_buf.f_bavail * statfs_buf.f_frsize;
 #endif
         blockSize = statfs_buf.f_bsize;
+#ifdef Q_OS_RISCOS
+        readOnly = false;
+#else
 #if defined(Q_OS_ANDROID) || defined (Q_OS_BSD4)
 #if defined(_STATFS_F_FLAGS)
         readOnly = (statfs_buf.f_flags & ST_RDONLY) != 0;
@@ -539,6 +547,7 @@
 #else
         readOnly = (statfs_buf.f_flag & ST_RDONLY) != 0;
 #endif
+#endif
     }
 }
 
