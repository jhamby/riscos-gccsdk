--- src/corelib/plugin/qpluginloader.cpp.orig	2021-08-14 22:19:58.456129266 -0700
+++ src/corelib/plugin/qpluginloader.cpp	2021-08-14 22:31:45.122348982 -0700
@@ -298,7 +298,9 @@
         paths.append(fileName.left(slash)); // don't include the '/'
     } else {
         paths = QCoreApplication::libraryPaths();
+#ifndef __riscos__
         paths.prepend(QStringLiteral(".")); // search in current dir first
+#endif
     }
 
     foreach (const QString &path, paths) {
