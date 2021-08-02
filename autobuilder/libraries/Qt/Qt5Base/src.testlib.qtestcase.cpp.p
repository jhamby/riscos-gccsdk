--- src/testlib/qtestcase.cpp.orig	2021-07-30 14:36:11.110651539 -0700
+++ src/testlib/qtestcase.cpp	2021-07-30 14:36:51.422950535 -0700
@@ -2667,11 +2667,11 @@
     act.sa_handler = FatalSignalHandler::signal;
 
     // Remove the handler after it is invoked.
-#if !defined(Q_OS_INTEGRITY)
+#if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_RISCOS)
     act.sa_flags = SA_RESETHAND;
 #endif
 
-#ifdef SA_ONSTACK
+#if defined(SA_ONSTACK) && !defined(Q_OS_RISCOS)
     // Let the signal handlers use an alternate stack
     // This is necessary if SIGSEGV is to catch a stack overflow
 #  if defined(Q_CC_GNU) && defined(Q_OF_ELF)
