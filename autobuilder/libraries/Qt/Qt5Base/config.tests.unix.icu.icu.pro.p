--- config.tests/unix/icu/icu.pro.orig	2015-02-17 04:56:55.000000000 +0000
+++ config.tests/unix/icu/icu.pro	2015-04-12 13:43:51.632657032 +0100
@@ -15,3 +15,7 @@
 } else {
     LIBS += -licui18n -licuuc -licudata
 }
+
+*riscos* {
+    LIBS += -licudata
+}
