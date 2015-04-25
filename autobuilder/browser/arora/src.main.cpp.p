--- src/main.cpp.orig	2015-04-10 20:47:29.380351907 +0100
+++ src/main.cpp	2015-04-11 18:07:10.248022422 +0100
@@ -18,24 +18,53 @@
  */
 
 #include "browserapplication.h"
+#include <QSslConfiguration>
+#include <QSslCipher>
 
 #ifdef Q_OS_WIN
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
+#ifndef Q_OS_RISCOS
+void setReasonableSslConfiguration() {
+    QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
+    QList<QSslCipher> wantedCiphers;
+    Q_FOREACH(const QSslCipher& cipher, conf.ciphers()) {
+        if(cipher.usedBits()>=128) {
+	    wantedCiphers << cipher;
+	}
+    }
+    conf.setCiphers(wantedCiphers);
+    QSslConfiguration::setDefaultConfiguration(conf);
+}
+#endif
+
 int main(int argc, char **argv)
 {
     Q_INIT_RESOURCE(htmls);
     Q_INIT_RESOURCE(data);
-#ifdef Q_WS_X11
-    QApplication::setGraphicsSystem(QString::fromLatin1("raster"));
-#endif
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
+#ifndef __riscos__
+    setReasonableSslConfiguration();
+#endif
     application.newMainWindow();
     return application.exec();
 }
