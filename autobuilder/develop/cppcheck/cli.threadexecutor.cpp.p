--- cli/threadexecutor.cpp.orig	2022-07-16 09:42:01.848267719 +0100
+++ cli/threadexecutor.cpp	2022-07-16 09:44:14.080252172 +0100
@@ -202,7 +202,7 @@
 
 bool ThreadExecutor::checkLoadAverage(size_t nchildren)
 {
-#if defined(__CYGWIN__) || defined(__QNX__) || defined(__HAIKU__)  // getloadavg() is unsupported on Cygwin, Qnx, Haiku.
+#if defined(__CYGWIN__) || defined(__QNX__) || defined(__HAIKU__) || defined(__riscos__) // getloadavg() is unsupported on Cygwin, Qnx, Haiku.
     return true;
 #else
     if (!nchildren || !mSettings.loadAverage) {
