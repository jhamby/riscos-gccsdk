--- src/corelib/io/qfilesystemengine_unix.cpp.orig	2021-08-16 19:58:38.838789409 -0700
+++ src/corelib/io/qfilesystemengine_unix.cpp	2021-08-16 20:00:33.935334012 -0700
@@ -821,7 +821,7 @@
 //static
 QString QFileSystemEngine::resolveUserName(uint userId)
 {
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_RISCOS)
     int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
@@ -830,7 +830,7 @@
 
 #if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_WASM)
     struct passwd *pw = 0;
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS)
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS) && !defined(Q_OS_RISCOS)
     struct passwd entry;
     getpwuid_r(userId, &entry, buf.data(), buf.size(), &pw);
 #else
@@ -845,7 +845,7 @@
 //static
 QString QFileSystemEngine::resolveGroupName(uint groupId)
 {
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_RISCOS)
     int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
@@ -854,7 +854,7 @@
 
 #if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_WASM)
     struct group *gr = 0;
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS) && (!defined(Q_OS_ANDROID) || defined(Q_OS_ANDROID) && (__ANDROID_API__ >= 24))
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS) && (!defined(Q_OS_ANDROID) || defined(Q_OS_ANDROID) && (__ANDROID_API__ >= 24)) && !defined(Q_OS_RISCOS)
     size_max = sysconf(_SC_GETGR_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
