--- Tools/ImageDiff/ImageDiff.pro.orig	2014-05-15 18:12:22.000000000 +0100
+++ Tools/ImageDiff/ImageDiff.pro	2014-08-23 19:53:29.564208133 +0100
@@ -12,3 +12,7 @@
 QT = core gui widgets
 
 SOURCES = qt/ImageDiff.cpp
+
+arm*riscos*: {
+    LIBS += -licui18n -licuuc -licudata
+}
