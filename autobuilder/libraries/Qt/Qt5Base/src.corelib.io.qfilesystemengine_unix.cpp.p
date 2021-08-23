--- ./src/corelib/io/qfilesystemengine_unix.cpp.orig	2020-10-27 01:02:11.000000000 -0700
+++ ./src/corelib/io/qfilesystemengine_unix.cpp	2021-08-22 14:58:54.331094208 -0700
@@ -798,7 +798,7 @@
 //static
 QString QFileSystemEngine::resolveUserName(uint userId)
 {
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_RISCOS)
     int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
@@ -807,7 +807,7 @@
 
 #if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_WASM)
     struct passwd *pw = nullptr;
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS)
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS) && !defined(Q_OS_RISCOS)
     struct passwd entry;
     getpwuid_r(userId, &entry, buf.data(), buf.size(), &pw);
 #else
@@ -824,7 +824,7 @@
 //static
 QString QFileSystemEngine::resolveGroupName(uint groupId)
 {
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_RISCOS)
     int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
@@ -833,7 +833,7 @@
 
 #if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_WASM)
     struct group *gr = nullptr;
-#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS) && (!defined(Q_OS_ANDROID) || defined(Q_OS_ANDROID) && (__ANDROID_API__ >= 24))
+#if QT_CONFIG(thread) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS) && (!defined(Q_OS_ANDROID) || defined(Q_OS_ANDROID) && (__ANDROID_API__ >= 24)) && !defined(Q_OS_RISCOS)
     size_max = sysconf(_SC_GETGR_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
