--- src/corelib/io/qfsfileengine_unix.cpp.orig	2015-06-29 21:05:01.000000000 +0100
+++ src/corelib/io/qfsfileengine_unix.cpp	2015-12-27 20:14:44.038650080 +0000
@@ -39,6 +39,10 @@
 #include "qfilesystemengine_p.h"
 #include "qcoreapplication.h"
 
+#if defined(Q_OS_RISCOS)
+#include "qdynamicarea.h"
+#endif
+
 #ifndef QT_NO_FSFILEENGINE
 
 #include "qfile.h"
@@ -266,7 +270,7 @@
 bool QFSFileEnginePrivate::nativeSyncToDisk()
 {
     Q_Q(QFSFileEngine);
-#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
+#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0 && !defined(__riscos__)
     const int ret = fdatasync(nativeHandle());
 #else
     const int ret = fsync(nativeHandle());
@@ -727,8 +731,12 @@
     QT_OFF_T realOffset = QT_OFF_T(offset);
     realOffset &= ~(QT_OFF_T(pageSize - 1));
 
+#if defined(Q_OS_RISCOS)
+    void *mapAddress = qt_mmap(realSize, nativeHandle(), realOffset);
+#else
     void *mapAddress = QT_MMAP((void*)0, realSize,
                    access, sharemode, nativeHandle(), realOffset);
+#endif
     if (MAP_FAILED != mapAddress) {
         uchar *address = extra + static_cast<uchar*>(mapAddress);
         maps[address] = QPair<int,size_t>(extra, realSize);
@@ -763,10 +771,14 @@
 
     uchar *start = ptr - maps[ptr].first;
     size_t len = maps[ptr].second;
+#if defined(Q_OS_RISCOS)
+    qt_munmap(start, len);
+#else
     if (-1 == munmap(start, len)) {
         q->setError(QFile::UnspecifiedError, qt_error_string(errno));
         return false;
     }
+#endif
     maps.remove(ptr);
     return true;
 #else
