--- lib/Support/Unix/Signals.inc.orig	2014-04-28 05:05:08.000000000 +0100
+++ lib/Support/Unix/Signals.inc	2015-02-01 14:05:31.894608734 +0000
@@ -91,7 +91,11 @@
   struct sigaction NewHandler;
 
   NewHandler.sa_handler = SignalHandler;
+#ifdef __riscos__
+  NewHandler.sa_flags = 0;
+#else
   NewHandler.sa_flags = SA_NODEFER|SA_RESETHAND;
+#endif
   sigemptyset(&NewHandler.sa_mask);
 
   // Install the new handler, save the old one in RegisteredSignalInfo.
