--- src/gui/text/qrawfont.cpp.orig	2014-05-15 18:12:15.000000000 +0100
+++ src/gui/text/qrawfont.cpp	2014-06-12 20:25:55.305656043 +0100
@@ -665,7 +665,7 @@
 */
 bool QRawFont::supportsCharacter(QChar character) const
 {
-    return supportsCharacter(character.unicode());
+    return supportsCharacter(RISCOS_DEREF_QCHAR(character.unicode()));
 }
 
 /*!
