--- src/src.pri.orig	2014-04-06 19:17:48.000000000 +0100
+++ src/src.pri	2015-05-02 13:44:59.835961159 +0100
@@ -143,3 +143,7 @@
 win32 {
     LIBS += -ladvapi32
 }
+
+*riscos* {
+    LIBS += -lWebKit1
+}
