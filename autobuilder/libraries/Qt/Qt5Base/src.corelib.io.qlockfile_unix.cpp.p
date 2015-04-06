--- src/corelib/io/qlockfile_unix.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qlockfile_unix.cpp	2014-05-27 19:55:16.338522841 +0100
@@ -87,7 +87,7 @@
     if (!file.open())
         return 0;
     const int fd = file.d_func()->engine()->handle();
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return 0;
 #endif
@@ -124,7 +124,7 @@
 
 static bool setNativeLocks(int fd)
 {
-#if defined(LOCK_EX) && defined(LOCK_NB)
+#if defined(LOCK_EX) && defined(LOCK_NB) && !defined(Q_OS_RISCOS)
     if (flock(fd, LOCK_EX | LOCK_NB) == -1) // other threads, and other processes on a local fs
         return false;
 #endif
