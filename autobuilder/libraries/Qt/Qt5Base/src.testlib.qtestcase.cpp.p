--- src/testlib/qtestcase.cpp.orig	2015-02-17 04:56:53.000000000 +0000
+++ src/testlib/qtestcase.cpp	2015-04-12 13:43:51.648657032 +0100
@@ -2294,7 +2294,7 @@
     act.sa_handler = FatalSignalHandler::signal;
 
     // Remove the handler after it is invoked.
-#if !defined(Q_OS_INTEGRITY)
+#if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_RISCOS)
     act.sa_flags = SA_RESETHAND;
 #endif
     // Block all fatal signals in our signal handler so we don't try to close
