--- src/plugins/platforms/platforms.pro.orig	2014-05-15 18:12:18.000000000 +0100
+++ src/plugins/platforms/platforms.pro	2014-05-27 19:55:16.342522841 +0100
@@ -26,6 +26,10 @@
     SUBDIRS += qnx
 }
 
+*riscos* {
+    SUBDIRS += riscos
+}
+
 contains(QT_CONFIG, eglfs) {
     SUBDIRS += eglfs
     SUBDIRS += minimalegl
