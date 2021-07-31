--- cli/threadexecutor.cpp.orig	2016-03-25 09:27:43.486113352 +0000
+++ cli/threadexecutor.cpp	2016-03-25 09:28:08.990113629 +0000
@@ -143,7 +143,7 @@
 
 bool ThreadExecutor::checkLoadAverage(size_t nchildren)
 {
-#if defined(__CYGWIN__) || defined(__QNX__)  // getloadavg() is unsupported on Cygwin, Qnx.
+#if defined(__CYGWIN__) || defined(__QNX__) || defined(__riscos__) // getloadavg() is unsupported on Cygwin, Qnx.
     return true;
 #else
     if (!nchildren || !_settings.loadAverage) {
