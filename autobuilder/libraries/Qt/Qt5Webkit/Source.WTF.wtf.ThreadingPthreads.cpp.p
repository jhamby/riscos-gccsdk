--- Source/WTF/wtf/ThreadingPthreads.cpp.orig	2014-05-15 18:12:23.000000000 +0100
+++ Source/WTF/wtf/ThreadingPthreads.cpp	2015-03-14 19:12:48.648135437 +0000
@@ -215,7 +215,11 @@
 void initializeCurrentThreadInternal(const char* threadName)
 {
 #if HAVE(PTHREAD_SETNAME_NP)
+#if OS(RISCOS)
+    pthread_setname_np(pthread_self(), threadName);
+#else
     pthread_setname_np(threadName);
+#endif
 #elif OS(QNX)
     pthread_setname_np(pthread_self(), threadName);
 #else
