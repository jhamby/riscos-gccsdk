--- src/corelib/io/qlockfile_unix.cpp.orig	2015-02-17 04:56:51.000000000 +0000
+++ src/corelib/io/qlockfile_unix.cpp	2015-04-12 13:43:51.612657031 +0100
@@ -79,7 +79,7 @@
     if (!file.open())
         return 0;
     const int fd = file.d_func()->engine()->handle();
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return 0;
 #endif
@@ -116,7 +116,7 @@
 
 static bool setNativeLocks(int fd)
 {
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return false;
 #endif
