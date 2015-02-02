--- lib/Support/LockFileManager.cpp.orig	2014-07-06 18:43:13.000000000 +0100
+++ lib/Support/LockFileManager.cpp	2015-01-21 20:16:40.120263471 +0000
@@ -58,7 +58,7 @@
 }
 
 bool LockFileManager::processStillExecuting(StringRef Hostname, int PID) {
-#if LLVM_ON_UNIX && !defined(__ANDROID__)
+#if LLVM_ON_UNIX && !defined(__ANDROID__) && !defined(__riscos__)
   char MyHostname[256];
   MyHostname[255] = 0;
   MyHostname[0] = 0;
