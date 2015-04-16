--- src/corelib/io/qstorageinfo_unix.cpp.orig	2015-02-17 04:56:51.000000000 +0000
+++ src/corelib/io/qstorageinfo_unix.cpp	2015-04-12 19:54:20.805294524 +0100
@@ -62,6 +62,8 @@
 #  include <sys/statvfs.h>
 #elif defined(Q_OS_SOLARIS)
 #  include <sys/mnttab.h>
+#elif defined(Q_OS_RISCOS)
+#  include <sys/vfs.h>
 #else
 #  include <sys/statvfs.h>
 #endif
@@ -75,6 +77,9 @@
 #  if !defined(ST_RDONLY)
 #    define ST_RDONLY 1 // hack for missing define on Android
 #  endif
+#elif defined(Q_OS_RISCOS)
+#  define QT_STATFS    ::statfs
+#  define QT_STATFSBUF struct statfs
 #else
 #  if defined(QT_LARGEFILE_SUPPORT)
 #    define QT_STATFSBUF struct statvfs64
@@ -118,7 +123,7 @@
     inline QByteArray fileSystemType() const;
     inline QByteArray device() const;
 private:
-#if defined(Q_OS_BSD4)
+#if defined(Q_OS_BSD4) || defined(Q_OS_RISCOS)
     struct statfs *stat_buf;
     int entryCount;
     int currentIndex;
@@ -418,6 +423,9 @@
         bytesTotal = statfs_buf.f_blocks * statfs_buf.f_bsize;
         bytesFree = statfs_buf.f_bfree * statfs_buf.f_bsize;
         bytesAvailable = statfs_buf.f_bavail * statfs_buf.f_bsize;
+#ifdef Q_OS_RISCOS
+	readOnly = false;
+#else
 #if defined(Q_OS_ANDROID)
 #if defined(_STATFS_F_FLAGS)
         readOnly = (statfs_buf.f_flags & ST_RDONLY) != 0;
@@ -425,6 +433,7 @@
 #else
         readOnly = (statfs_buf.f_flag & ST_RDONLY) != 0;
 #endif
+#endif
     }
 }
 
