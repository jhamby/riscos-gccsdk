--- Tools/QtTestBrowser/qttestbrowser.cpp.orig	2014-05-15 18:12:22.000000000 +0100
+++ Tools/QtTestBrowser/qttestbrowser.cpp	2014-06-23 17:21:38.247162320 +0100
@@ -44,6 +44,15 @@
 #include <QFileInfo>
 #include <QFontDatabase>
 
+#ifdef __riscos__
+#include <features.h>
+
+// Override the __stack_size weak symbol in Unixlib.
+// Force it to be visible incase all symbols are hidden by default in
+// build system.
+int __stack_size __attribute__ ((visibility ("default"))) = 1024 * 1024; // 1MB stack
+#endif
+
 int launcherMain(const QApplication& app)
 {
 #ifndef NDEBUG
