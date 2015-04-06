--- config.tests/unix/icu/icu.pro.orig	2014-05-15 18:12:18.000000000 +0100
+++ config.tests/unix/icu/icu.pro	2014-08-22 19:04:34.383630507 +0100
@@ -14,3 +14,7 @@
 } else {
     LIBS += -licui18n -licuuc
 }
+
+*riscos* {
+    LIBS += -licudata
+}
