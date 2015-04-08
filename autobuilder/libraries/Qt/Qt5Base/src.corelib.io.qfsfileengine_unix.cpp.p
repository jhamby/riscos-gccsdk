--- src/corelib/io/qfsfileengine_unix.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qfsfileengine_unix.cpp	2015-04-06 14:30:04.062590857 +0100
@@ -274,7 +274,7 @@
 bool QFSFileEnginePrivate::nativeSyncToDisk()
 {
     Q_Q(QFSFileEngine);
-#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
+#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0 && !defined(__riscos__)
     const int ret = fdatasync(nativeHandle());
 #else
     const int ret = fsync(nativeHandle());
