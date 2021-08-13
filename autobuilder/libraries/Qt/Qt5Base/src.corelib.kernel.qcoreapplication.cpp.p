--- src/corelib/kernel/qcoreapplication.cpp.orig	2015-10-12 21:35:31.000000000 -0700
+++ src/corelib/kernel/qcoreapplication.cpp	2021-08-14 13:12:03.574258570 -0700
@@ -2484,7 +2484,7 @@
 
         const QByteArray libPathEnv = qgetenv("QT_PLUGIN_PATH");
         if (!libPathEnv.isEmpty()) {
-#if defined(Q_OS_WIN)
+#if defined(Q_OS_WIN) || defined(Q_OS_RISCOS)
             QLatin1Char pathSep(';');
 #else
             QLatin1Char pathSep(':');
