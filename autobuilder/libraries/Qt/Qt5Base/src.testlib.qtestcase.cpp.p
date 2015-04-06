--- src/testlib/qtestcase.cpp.orig	2014-05-15 18:12:18.000000000 +0100
+++ src/testlib/qtestcase.cpp	2014-05-27 19:55:16.366522842 +0100
@@ -2284,7 +2284,7 @@
     act.sa_handler = FatalSignalHandler::signal;
 
     // Remove the handler after it is invoked.
-#if !defined(Q_OS_INTEGRITY)
+#if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_RISCOS)
     act.sa_flags = SA_RESETHAND;
 #endif
     // Block all fatal signals in our signal handler so we don't try to close
