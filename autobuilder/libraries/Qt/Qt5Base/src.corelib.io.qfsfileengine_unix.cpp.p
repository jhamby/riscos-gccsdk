--- src/corelib/io/qfsfileengine_unix.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qfsfileengine_unix.cpp	2014-05-27 19:55:16.378522842 +0100
@@ -274,7 +274,7 @@
 bool QFSFileEnginePrivate::nativeSyncToDisk()
 {
     Q_Q(QFSFileEngine);
-#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
+#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0 && !defined(__riscos__)
     const int ret = fdatasync(nativeHandle());
 #else
     const int ret = fsync(nativeHandle());
@@ -729,7 +729,7 @@
     QT_OFF_T realOffset = QT_OFF_T(offset);
     realOffset &= ~(QT_OFF_T(pageSize - 1));
 
-    void *mapAddress = QT_MMAP((void*)0, realSize,
+    void *mapAddress = QT_MMAP((__caddr_t)0, realSize,
                    access, MAP_SHARED, nativeHandle(), realOffset);
     if (MAP_FAILED != mapAddress) {
         uchar *address = extra + static_cast<uchar*>(mapAddress);
@@ -765,7 +765,7 @@
 
     uchar *start = ptr - maps[ptr].first;
     size_t len = maps[ptr].second;
-    if (-1 == munmap(start, len)) {
+    if (-1 == munmap((__caddr_t)start, len)) {
         q->setError(QFile::UnspecifiedError, qt_error_string(errno));
         return false;
     }
