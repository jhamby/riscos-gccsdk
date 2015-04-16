--- src/corelib/thread/qthread_unix.cpp.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/thread/qthread_unix.cpp	2015-04-12 13:43:51.608657031 +0100
@@ -273,7 +273,7 @@
 
 #ifndef QT_NO_THREAD
 
-#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
+#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX) || defined(Q_OS_RISCOS))
 static void setCurrentThreadName(pthread_t threadId, const char *name)
 {
 #  if defined(Q_OS_LINUX) && !defined(QT_LINUXBASE)
@@ -282,7 +282,7 @@
 #  elif defined(Q_OS_MAC)
     Q_UNUSED(threadId);
     pthread_setname_np(name);
-#  elif defined(Q_OS_QNX)
+#  elif defined(Q_OS_QNX) || defined(Q_OS_RISCOS)
     pthread_setname_np(threadId, name);
 #  endif
 }
@@ -318,7 +318,7 @@
     else
         createEventDispatcher(data);
 
-#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
+#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX) || defined(Q_OS_RISCOS))
     // sets the name of the current thread.
     QString objectName = thr->objectName();
 
@@ -446,6 +446,8 @@
     // as of aug 2008 VxWorks < 6.6 only supports one single core CPU
     cores = 1;
 #  endif
+#elif defined(Q_OS_RISCOS)
+    cores = 1;
 #else
     // the rest: Linux, Solaris, AIX, Tru64
     cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
