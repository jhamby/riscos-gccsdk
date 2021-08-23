--- src/testlib/qtestcase.cpp.orig	2021-08-22 12:02:10.621631511 -0700
+++ src/testlib/qtestcase.cpp	2021-08-22 14:39:26.723115702 -0700
@@ -1647,13 +1647,13 @@
         act.sa_handler = FatalSignalHandler::signal;
 
         // Remove the handler after it is invoked.
-#  if !defined(Q_OS_INTEGRITY)
+#  if !defined(Q_OS_INTEGRITY) && !defined(Q_OS_RISCOS)
         act.sa_flags = SA_RESETHAND;
 #  endif
 
     // tvOS/watchOS both define SA_ONSTACK (in sys/signal.h) but mark sigaltstack() as
     // unavailable (__WATCHOS_PROHIBITED __TVOS_PROHIBITED in signal.h)
-#  if defined(SA_ONSTACK) && !defined(Q_OS_TVOS) && !defined(Q_OS_WATCHOS)
+#  if defined(SA_ONSTACK) && !defined(Q_OS_TVOS) && !defined(Q_OS_WATCHOS) && !defined(Q_OS_RISCOS)
         // Let the signal handlers use an alternate stack
         // This is necessary if SIGSEGV is to catch a stack overflow
 #    if defined(Q_CC_GNU) && defined(Q_OF_ELF)
