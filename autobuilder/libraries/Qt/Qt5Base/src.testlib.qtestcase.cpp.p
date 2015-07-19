--- src/testlib/qtestcase.cpp.orig	2015-06-29 21:04:38.000000000 +0100
+++ src/testlib/qtestcase.cpp	2015-07-15 18:24:20.410735903 +0100
@@ -2446,7 +2446,7 @@
     act.sa_handler = FatalSignalHandler::signal;
 
     // Remove the handler after it is invoked.
-#if !defined(Q_OS_INTEGRITY)
+#if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_RISCOS)
     act.sa_flags = SA_RESETHAND;
 #endif
     // Block all fatal signals in our signal handler so we don't try to close
