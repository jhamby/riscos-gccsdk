--- src/corelib/io/qfsfileengine_unix.cpp.orig	2021-08-22 16:52:05.811593035 -0700
+++ src/corelib/io/qfsfileengine_unix.cpp	2021-08-22 16:51:31.683390172 -0700
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
@@ -589,6 +593,7 @@
             && (QT_OFF_T(size) > metaData.size() - QT_OFF_T(offset)))
         qWarning("QFSFileEngine::map: Mapping a file beyond its size is not portable");
 
+#if !defined(Q_OS_RISCOS)
     int access = 0;
     if (openMode & QIODevice::ReadOnly) access |= PROT_READ;
     if (openMode & QIODevice::WriteOnly) access |= PROT_WRITE;
@@ -598,6 +603,7 @@
         sharemode = MAP_PRIVATE;
         access |= PROT_WRITE;
     }
+#endif
 
 #if defined(Q_OS_INTEGRITY)
     int pageSize = sysconf(_SC_PAGESIZE);
@@ -615,8 +621,12 @@
     QT_OFF_T realOffset = QT_OFF_T(offset);
     realOffset &= ~(QT_OFF_T(pageSize - 1));
 
+#if defined(Q_OS_RISCOS)
+    void *mapAddress = qt_mmap(realSize, nativeHandle(), realOffset);
+#else
     void *mapAddress = QT_MMAP((void*)nullptr, realSize,
                    access, sharemode, nativeHandle(), realOffset);
+#endif
     if (MAP_FAILED != mapAddress) {
         uchar *address = extra + static_cast<uchar*>(mapAddress);
         maps[address] = QPair<int,size_t>(extra, realSize);
@@ -651,10 +661,14 @@
 
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
