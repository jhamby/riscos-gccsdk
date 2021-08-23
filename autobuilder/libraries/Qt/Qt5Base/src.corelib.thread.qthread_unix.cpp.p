--- ./src/corelib/thread/qthread_unix.cpp.orig	2020-10-27 01:02:11.000000000 -0700
+++ ./src/corelib/thread/qthread_unix.cpp	2021-08-22 15:01:15.151934776 -0700
@@ -265,14 +265,14 @@
 
 #if QT_CONFIG(thread)
 
-#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
+#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX) || defined(Q_OS_RISCOS))
 static void setCurrentThreadName(const char *name)
 {
 #  if defined(Q_OS_LINUX) && !defined(QT_LINUXBASE)
     prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0);
 #  elif defined(Q_OS_MAC)
     pthread_setname_np(name);
-#  elif defined(Q_OS_QNX)
+#  elif defined(Q_OS_QNX) || defined(Q_OS_RISCOS)
     pthread_setname_np(pthread_self(), name);
 #  endif
 }
@@ -309,7 +309,7 @@
 
         data->ensureEventDispatcher();
 
-#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
+#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX) || defined(Q_OS_RISCOS))
         {
             // Sets the name of the current thread. We can only do this
             // when the thread is starting, as we don't have a cross
@@ -473,6 +473,8 @@
 #  endif
 #elif defined(Q_OS_WASM)
     cores = QThreadPrivate::idealThreadCount;
+#elif defined(Q_OS_RISCOS)
+    cores = 1;
 #else
     // the rest: Linux, Solaris, AIX, Tru64
     cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
