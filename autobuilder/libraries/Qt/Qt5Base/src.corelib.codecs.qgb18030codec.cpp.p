--- src/corelib/codecs/qgb18030codec.cpp.orig	2013-08-25 19:03:37.000000000 +0100
+++ src/corelib/codecs/qgb18030codec.cpp	2013-11-17 16:45:08.184780354 +0000
@@ -414,7 +414,7 @@
         if (ch.row() == 0x00 && ch.cell() < 0x80) {
             // ASCII
             *cursor++ = ch.cell();
-        } else if (qt_UnicodeToGbk(ch.unicode(), buf) == 2) {
+        } else if (qt_UnicodeToGbk(RISCOS_DEREF_QCHAR(ch.unicode()), buf) == 2) {
             *cursor++ = buf[0];
             *cursor++ = buf[1];
         } else {
@@ -564,7 +564,7 @@
         if (ch.row() == 0x00 && ch.cell() < 0x80) {
             // ASCII
             *cursor++ = ch.cell();
-        } else if ((qt_UnicodeToGbk(ch.unicode(), buf) == 2) &&
+        } else if ((qt_UnicodeToGbk(RISCOS_DEREF_QCHAR(ch.unicode()), buf) == 2) &&
                     (buf[0] >= 0xA1) && (buf[1] >= 0xA1)) {
             *cursor++ = buf[0];
             *cursor++ = buf[1];
