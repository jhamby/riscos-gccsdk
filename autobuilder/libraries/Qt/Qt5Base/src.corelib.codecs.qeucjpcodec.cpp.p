--- src/corelib/codecs/qeucjpcodec.cpp.orig	2013-08-25 19:03:36.000000000 +0100
+++ src/corelib/codecs/qeucjpcodec.cpp	2013-11-17 16:47:09.860781931 +0000
@@ -120,7 +120,7 @@
     for (int i = 0; i < len; i++) {
         QChar ch = uc[i];
         uint j;
-        if (ch.unicode() < 0x80) {
+        if (RISCOS_DEREF_QCHAR(ch.unicode()) < 0x80) {
             // ASCII
             *cursor++ = ch.cell();
         } else if ((j = conv->unicodeToJisx0201(ch.row(), ch.cell())) != 0) {
