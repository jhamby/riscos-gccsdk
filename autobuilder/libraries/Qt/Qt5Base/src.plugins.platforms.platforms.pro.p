--- src/plugins/platforms/platforms.pro.orig	2021-08-16 13:20:34.221132899 -0700
+++ src/plugins/platforms/platforms.pro	2021-08-16 13:23:36.401751261 -0700
@@ -25,6 +25,10 @@
     SUBDIRS += qnx
 }
 
+*riscos* {
+    SUBDIRS += riscos
+}
+
 qtConfig(eglfs) {
     SUBDIRS += eglfs
     SUBDIRS += minimalegl
