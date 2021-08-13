--- src/corelib/io/qstandardpaths.cpp.orig	2015-10-12 21:35:31.000000000 -0700
+++ src/corelib/io/qstandardpaths.cpp	2021-08-14 13:24:29.638112523 -0700
@@ -523,7 +523,7 @@
     QStringList searchPaths = paths;
     if (paths.isEmpty()) {
         QByteArray pEnv = qgetenv("PATH");
-#if defined(Q_OS_WIN)
+#if defined(Q_OS_WIN) || defined(Q_OS_RISCOS)
         const QLatin1Char pathSep(';');
 #else
         const QLatin1Char pathSep(':');
