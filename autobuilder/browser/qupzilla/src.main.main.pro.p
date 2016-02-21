--- src/main/main.pro.orig	2016-02-21 11:31:24.890836259 +0000
+++ src/main/main.pro	2016-02-21 11:31:40.354836275 +0000
@@ -1,7 +1,7 @@
 include(../defines.pri)
 
 isEqual(QT_MAJOR_VERSION, 5) {
-    QT += webkitwidgets network widgets printsupport sql script
+    QT += webkitwidgets network widgets printsupport sql
 } else {
     QT += core gui webkit sql network script
 }
