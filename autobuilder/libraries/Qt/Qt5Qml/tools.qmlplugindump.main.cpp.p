--- tools/qmlplugindump/main.cpp.orig	2017-09-12 01:45:46.000000000 -0700
+++ tools/qmlplugindump/main.cpp	2021-08-14 23:30:20.598202045 -0700
@@ -931,7 +931,11 @@
 
 void printDebugMessage(QtMsgType, const QMessageLogContext &, const QString &msg)
 {
+#ifdef Q_OS_RISCOS
+    std::cerr << msg << std::endl;
+#else
     std::wcerr << msg << std::endl;
+#endif
     // In case of QtFatalMsg the calling code will abort() when appropriate.
 }
 
