--- src/corelib/global/qglobal.h.orig	2015-06-29 21:04:58.000000000 +0100
+++ src/corelib/global/qglobal.h	2015-07-18 11:29:18.225956100 +0100
@@ -589,6 +589,12 @@
 #  define QT_NO_PROCESS
 #endif
 
+#if defined(Q_OS_RISCOS)
+#  define QT_NO_CRASHHANDLER
+#  define QT_NO_SYSTEMSEMAPHORE
+#  define QT_NO_SHAREDMEMORY
+#endif
+
 inline void qt_noop(void) {}
 
 /* These wrap try/catch so we can switch off exceptions later.
