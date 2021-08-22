--- src/corelib/io/qstorageinfo_unix.cpp.orig	2021-05-17 23:43:52.000000000 -0700
+++ src/corelib/io/qstorageinfo_unix.cpp	2021-08-16 13:57:35.985112203 -0700
@@ -70,6 +70,8 @@
 #  include <VolumeRoster.h>
 #  include <fs_info.h>
 #  include <sys/statvfs.h>
+#elif defined(Q_OS_RISCOS)
+#  include <sys/vfs.h>
 #else
 #  include <sys/statvfs.h>
 #endif
@@ -98,6 +100,9 @@
 #elif defined(Q_OS_HAIKU)
 #  define QT_STATFSBUF struct statvfs
 #  define QT_STATFS    ::statvfs
+#elif defined(Q_OS_RISCOS)
+#  define QT_STATFS    ::statfs
+#  define QT_STATFSBUF struct statfs
 #else
 #  if defined(QT_LARGEFILE_SUPPORT)
 #    define QT_STATFSBUF struct statvfs64
@@ -131,7 +136,7 @@
     inline QByteArray options() const;
     inline QByteArray subvolume() const;
 private:
-#if defined(Q_OS_BSD4)
+#if defined(Q_OS_BSD4) || defined(Q_OS_RISCOS)
     QT_STATFSBUF *stat_buf;
     int entryCount;
     int currentIndex;
@@ -822,7 +827,7 @@
         valid = true;
         ready = true;
 
-#if defined(Q_OS_INTEGRITY) || (defined(Q_OS_BSD4) && !defined(Q_OS_NETBSD))
+#if defined(Q_OS_INTEGRITY) || (defined(Q_OS_BSD4) && !defined(Q_OS_NETBSD)) || defined(Q_OS_RISCOS)
         bytesTotal = statfs_buf.f_blocks * statfs_buf.f_bsize;
         bytesFree = statfs_buf.f_bfree * statfs_buf.f_bsize;
         bytesAvailable = statfs_buf.f_bavail * statfs_buf.f_bsize;
@@ -832,6 +837,9 @@
         bytesAvailable = statfs_buf.f_bavail * statfs_buf.f_frsize;
 #endif
         blockSize = statfs_buf.f_bsize;
+#ifdef Q_OS_RISCOS
+        readOnly = false;
+#else
 #if defined(Q_OS_ANDROID) || defined(Q_OS_BSD4) || defined(Q_OS_INTEGRITY)
 #if defined(_STATFS_F_FLAGS)
         readOnly = (statfs_buf.f_flags & ST_RDONLY) != 0;
@@ -839,6 +847,7 @@
 #else
         readOnly = (statfs_buf.f_flag & ST_RDONLY) != 0;
 #endif
+#endif
     }
 }
 
