--- tools/htmlToXBel/htmlToXBel.pro.orig	2010-09-27 03:42:17.000000000 +0100
+++ tools/htmlToXBel/htmlToXBel.pro	2015-04-24 20:39:39.191912224 +0100
@@ -6,7 +6,11 @@
 win32|os2: CONFIG += console
 mac:CONFIG -= app_bundle
 
-QT += network webkit
+contains(QT_VERSION, ^5.*) {
+    QT += network widgets webkitwidgets printsupport
+} else {
+    QT += network webkit
+}
 
 # Input
 SOURCES += main.cpp
@@ -33,3 +37,7 @@
     man-compress.depends = install_man
 }
 }
+
+*riscos* {
+    QMAKE_LFLAGS = -Wl,-rpath-link,$(WEBKITOUTPUTDIR)/Release/lib
+}
