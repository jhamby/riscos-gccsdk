--- src/corelib/io/qlockfile_unix.cpp.orig	2015-06-29 21:05:01.000000000 +0100
+++ src/corelib/io/qlockfile_unix.cpp	2015-07-15 18:24:19.938735896 +0100
@@ -89,7 +89,7 @@
     if (!file.open())
         return 0;
     const int fd = file.d_func()->engine()->handle();
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return 0;
 #endif
@@ -126,7 +126,7 @@
 
 static bool setNativeLocks(int fd)
 {
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return false;
 #endif
