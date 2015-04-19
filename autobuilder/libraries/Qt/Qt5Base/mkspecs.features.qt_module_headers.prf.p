--- mkspecs/features/qt_module_headers.prf.orig	2015-04-17 19:34:41.000000000 +0100
+++ mkspecs/features/qt_module_headers.prf	2015-04-17 19:36:25.090972836 +0100
@@ -76,3 +76,8 @@
 }
 
 CONFIG += qt_install_headers
+
+*riscos* {
+    SYNCQT.HEADER_CLASS += ../../include/QtCore/QDynamicArea
+    SYNCQT.HEADER_FILES += tools/qdynamicarea.h
+}
