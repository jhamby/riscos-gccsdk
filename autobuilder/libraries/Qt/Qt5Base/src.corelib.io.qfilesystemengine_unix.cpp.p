--- src/corelib/io/qfilesystemengine_unix.cpp.orig	2015-06-29 21:05:01.000000000 +0100
+++ src/corelib/io/qfilesystemengine_unix.cpp	2015-07-15 18:24:19.858735895 +0100
@@ -349,9 +349,13 @@
 QString QFileSystemEngine::resolveUserName(uint userId)
 {
 #if !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
+#ifdef Q_OS_RISCOS
+    int size_max = 1024;
+#else
     int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
+#endif
     QVarLengthArray<char, 1024> buf(size_max);
 #endif
 
@@ -373,18 +377,26 @@
 QString QFileSystemEngine::resolveGroupName(uint groupId)
 {
 #if !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD)
+#ifdef Q_OS_RISCOS
+    int size_max = 1024;
+#else
     int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
+#endif
     QVarLengthArray<char, 1024> buf(size_max);
 #endif
 
     struct group *gr = 0;
 #if !defined(Q_OS_INTEGRITY)
 #if !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_OPENBSD) && !defined(Q_OS_VXWORKS)
+#ifdef Q_OS_RISCOS
+    size_max = 1024;
+#else
     size_max = sysconf(_SC_GETGR_R_SIZE_MAX);
     if (size_max == -1)
         size_max = 1024;
+#endif
     buf.resize(size_max);
     struct group entry;
     // Some large systems have more members than the POSIX max size
