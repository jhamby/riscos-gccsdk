--- src/gui/kernel/qkeysequence.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/gui/kernel/qkeysequence.cpp	2014-05-27 19:55:16.366522842 +0100
@@ -919,7 +919,7 @@
             if (c.isPrint()) {
                 if (!found) {
                     c = c.toUpper();
-                    ret = QKeySequence(c.unicode() + Qt::ALT);
+                    ret = QKeySequence(RISCOS_DEREF_QCHAR(c.unicode()) + Qt::ALT);
 #ifdef QT_NO_DEBUG
                     return ret;
 #else
@@ -1124,7 +1124,7 @@
         } else
 #endif
         {
-            ret |= accel[0].toUpper().unicode();
+            ret |= RISCOS_DEREF_QCHAR(accel[0].toUpper().unicode());
         }
     } else if (accel[0] == QLatin1Char('f') && (fnum = accel.mid(1).toInt()) && (fnum >= 1) && (fnum <= 35)) {
         ret |= Qt::Key_F1 + fnum - 1;
