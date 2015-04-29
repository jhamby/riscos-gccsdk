--- src/src.pri.orig	2014-04-06 19:17:48.000000000 +0100
+++ src/src.pri	2015-04-29 13:52:50.663103359 +0100
@@ -143,3 +143,8 @@
 win32 {
     LIBS += -ladvapi32
 }
+
+*riscos* {
+    QMAKE_LFLAGS = -Wl,-rpath-link,$(WEBKITOUTPUTDIR)/Release/lib
+    LIBS += -L$(WEBKITOUTPUTDIR)/Release/lib -lWebKit1
+}
