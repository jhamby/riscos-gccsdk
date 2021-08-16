--- src/corelib/io/qlockfile_unix.cpp.orig	2017-09-06 05:13:54.000000000 -0700
+++ src/corelib/io/qlockfile_unix.cpp	2021-08-14 22:55:28.614301558 -0700
@@ -98,7 +98,7 @@
     if (!file.open())
         return 0;
     const int fd = file.d_func()->engine()->handle();
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return 0;
 #endif
@@ -149,7 +149,7 @@
 
 static bool setNativeLocks(const QString &fileName, int fd)
 {
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return false;
 #endif
@@ -205,7 +205,7 @@
     fileHandle = fd;
 
     // Sync to disk if possible. Ignore errors (e.g. not supported).
-#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
+#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0 && !defined(Q_OS_RISCOS)
     fdatasync(fileHandle);
 #else
     fsync(fileHandle);
