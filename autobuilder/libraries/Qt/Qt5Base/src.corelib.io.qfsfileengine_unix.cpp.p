--- src/corelib/io/qfsfileengine_unix.cpp.orig	2021-05-17 23:43:52.000000000 -0700
+++ src/corelib/io/qfsfileengine_unix.cpp	2021-08-16 13:40:30.128058387 -0700
@@ -45,6 +45,10 @@
 #include "qfilesystemengine_p.h"
 #include "qcoreapplication.h"
 
+#if defined(Q_OS_RISCOS)
+#include "qdynamicarea.h"
+#endif
+
 #ifndef QT_NO_FSFILEENGINE
 
 #include "qfile.h"
@@ -185,7 +189,7 @@
 {
     Q_Q(QFSFileEngine);
     int ret;
-#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
+#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0 && !defined(Q_OS_RISCOS)
     EINTR_LOOP(ret, fdatasync(nativeHandle()));
 #else
     EINTR_LOOP(ret, fsync(nativeHandle()));
@@ -692,8 +696,12 @@
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
@@ -728,10 +736,14 @@
 
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
