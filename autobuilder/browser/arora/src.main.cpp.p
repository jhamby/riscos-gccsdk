--- src/main.cpp.orig	2014-04-06 19:17:48.000000000 +0100
+++ src/main.cpp	2015-04-29 13:21:18.571078840 +0100
@@ -25,6 +25,15 @@
 #include "explorerstyle.h"
 #endif
 
+#ifdef Q_OS_RISCOS
+#include <features.h>
+
+// Override the __stack_size weak symbol in Unixlib.
+// Force it to be visible in case all symbols are hidden by default in
+// the build system.
+int __stack_size __attribute__ ((visibility ("default"))) = 1024 * 1024; // 1MB stack
+#endif
+
 void setReasonableSslConfiguration() {
     QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
     QList<QSslCipher> wantedCiphers;
@@ -42,8 +51,12 @@
     Q_INIT_RESOURCE(htmls);
     Q_INIT_RESOURCE(data);
     BrowserApplication application(argc, argv);
+    
+#ifndef Q_OS_RISCOS
     if (!application.isRunning())
         return 0;
+#endif
+
 #ifdef Q_OS_WIN
     application.setStyle(new ExplorerStyle);
 #endif
