--- ./src/corelib/thread/qthread_unix.cpp.orig	2017-09-06 05:13:54.000000000 -0700
+++ ./src/corelib/thread/qthread_unix.cpp	2021-08-14 22:42:38.635869364 -0700
@@ -299,7 +299,7 @@
 
 #ifndef QT_NO_THREAD
 
-#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
+#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX) || defined(Q_OS_RISCOS))
 static void setCurrentThreadName(pthread_t threadId, const char *name)
 {
 #  if defined(Q_OS_LINUX) && !defined(QT_LINUXBASE)
@@ -308,7 +308,7 @@
 #  elif defined(Q_OS_MAC)
     Q_UNUSED(threadId);
     pthread_setname_np(name);
-#  elif defined(Q_OS_QNX)
+#  elif defined(Q_OS_QNX) || defined(Q_OS_RISCOS)
     pthread_setname_np(threadId, name);
 #  endif
 }
@@ -344,7 +344,7 @@
     else
         createEventDispatcher(data);
 
-#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
+#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX) || defined(Q_OS_RISCOS))
     {
         // sets the name of the current thread.
         QString objectName = thr->objectName();
@@ -471,6 +471,8 @@
     // as of aug 2008 VxWorks < 6.6 only supports one single core CPU
     cores = 1;
 #  endif
+#elif defined(Q_OS_RISCOS)
+    cores = 1;
 #else
     // the rest: Linux, Solaris, AIX, Tru64
     cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
