--- src/gui/text/qtextengine.cpp.orig	2014-05-15 18:12:15.000000000 +0100
+++ src/gui/text/qtextengine.cpp	2014-06-12 20:23:23.897654081 +0100
@@ -2001,7 +2001,7 @@
     if (type >= Justification_Arabic_Normal) {
         QChar ch(0x640); // Kashida character
 
-        glyph_t kashidaGlyph = fe->glyphIndex(ch.unicode());
+        glyph_t kashidaGlyph = fe->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
         if (kashidaGlyph != 0) {
             QGlyphLayout g;
             g.numGlyphs = 1;
@@ -2554,9 +2554,9 @@
 
 static inline bool isRetainableControlCode(QChar c)
 {
-    return (c.unicode() >= 0x202a && c.unicode() <= 0x202e) // LRE, RLE, PDF, LRO, RLO
-            || (c.unicode() >= 0x200e && c.unicode() <= 0x200f) // LRM, RLM
-            || (c.unicode() >= 0x2066 && c.unicode() <= 0x2069); // LRM, RLM
+    return (RISCOS_DEREF_QCHAR(c.unicode()) >= 0x202a && RISCOS_DEREF_QCHAR(c.unicode()) <= 0x202e) // LRE, RLE, PDF, LRO, RLO
+            || (RISCOS_DEREF_QCHAR(c.unicode()) >= 0x200e && RISCOS_DEREF_QCHAR(c.unicode()) <= 0x200f) // LRM, RLM
+            || (RISCOS_DEREF_QCHAR(c.unicode()) >= 0x2066 && RISCOS_DEREF_QCHAR(c.unicode()) <= 0x2069); // LRM, RLM
 }
 
 static QString stringMidRetainingBidiCC(const QString &string,
@@ -2636,7 +2636,7 @@
 
         QChar ellipsisChar(0x2026);
 
-        glyph_t glyph = engine->glyphIndex(ellipsisChar.unicode());
+        glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ellipsisChar.unicode()));
 
         QGlyphLayout glyphs;
         glyphs.numGlyphs = 1;
