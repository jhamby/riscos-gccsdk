--- src/tools/uic/driver.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/uic/driver.cpp	2014-05-30 20:01:21.639922460 +0100
@@ -243,7 +243,7 @@
         QChar c = baseName.at(i);
         if (!isAnsiCCharacter(c)) {
             // Replace character by its unicode value
-            QString hex = QString::number(c.unicode(), 16);
+            QString hex = QString::number(RISCOS_DEREF_QCHAR(c.unicode()), 16);
             baseName.replace(i, 1, QLatin1Char('_') + hex + QLatin1Char('_'));
             i += hex.size() + 1;
         }
