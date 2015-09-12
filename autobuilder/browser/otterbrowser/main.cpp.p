--- src/main.cpp.orig	2015-04-30 01:40:58.902075449 +0100
+++ src/main.cpp	2015-04-30 01:41:52.134074268 +0100
@@ -26,6 +26,11 @@
 #include <QtCore/QLibraryInfo>
 #include <QtCore/QUrl>
 
+#ifdef __riscos__
+#include <features.h>
+int __stack_size __attribute__ ((visibility ("default"))) = 1024 * 1024; // 1MB stack
+#endif
+
 using namespace Otter;
 
 void otterMessageHander(QtMsgType type, const QMessageLogContext &context, const QString &message)
