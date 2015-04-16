--- src/corelib/io/qfsfileengine_unix.cpp.orig	2015-02-17 04:56:51.000000000 +0000
+++ src/corelib/io/qfsfileengine_unix.cpp	2015-04-12 13:43:51.580657030 +0100
@@ -266,7 +266,7 @@
 bool QFSFileEnginePrivate::nativeSyncToDisk()
 {
     Q_Q(QFSFileEngine);
-#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0
+#if defined(_POSIX_SYNCHRONIZED_IO) && _POSIX_SYNCHRONIZED_IO > 0 && !defined(__riscos__)
     const int ret = fdatasync(nativeHandle());
 #else
     const int ret = fsync(nativeHandle());
