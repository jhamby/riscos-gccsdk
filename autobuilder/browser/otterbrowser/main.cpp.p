--- src/main.cpp
+++ src/main.cpp
@@ -37,6 +37,11 @@
 #include <QtCore/QUrl>
 #include <QtWidgets/QMessageBox>
 
+#ifdef __riscos__
+#include <features.h>
+int __stack_size __attribute__ ((visibility ("default"))) = 1024 * 1024; // 1MB stack
+#endif
+
 using namespace Otter;
 
 #if !defined(Q_OS_WIN32)


