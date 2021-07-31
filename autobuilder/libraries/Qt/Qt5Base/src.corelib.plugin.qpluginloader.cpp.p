--- src/corelib/plugin/qpluginloader.cpp.orig	2015-10-31 11:21:28.970967185 +0000
+++ src/corelib/plugin/qpluginloader.cpp	2015-10-31 11:22:17.342966112 +0000
@@ -286,7 +286,9 @@
     const bool debug = qt_debug_component();
 
     QStringList paths = QCoreApplication::libraryPaths();
+#ifndef __riscos__
     paths.prepend(QStringLiteral("./")); // search in current dir first
+#endif
     foreach (const QString &path, paths) {
         foreach (const QString &prefix, prefixes) {
             foreach (const QString &suffix, suffixes) {
