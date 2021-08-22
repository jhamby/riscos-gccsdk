--- src/corelib/io/qlockfile_unix.cpp.orig	2021-05-17 23:43:52.000000000 -0700
+++ src/corelib/io/qlockfile_unix.cpp	2021-08-16 13:20:34.213132871 -0700
@@ -141,7 +141,7 @@
 
 static bool setNativeLocks(int fd)
 {
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return false;
 #else
@@ -183,7 +183,7 @@
     fileHandle = fd;
 
     // Sync to disk if possible. Ignore errors (e.g. not supported).
-#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
+#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0 && !defined(Q_OS_RISCOS)
     fdatasync(fileHandle);
 #else
     fsync(fileHandle);
