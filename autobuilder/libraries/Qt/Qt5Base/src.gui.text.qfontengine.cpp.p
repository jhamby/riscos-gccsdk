--- src/gui/text/qfontengine.cpp.orig	2014-05-15 18:12:15.000000000 +0100
+++ src/gui/text/qfontengine.cpp	2014-06-12 20:20:21.849651722 +0100
@@ -468,7 +468,7 @@
             if (glyphs.justifications[i].nKashidas) {
                 QChar ch(0x640); // Kashida character
 
-                glyph_t kashidaGlyph = glyphIndex(ch.unicode());
+                glyph_t kashidaGlyph = glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
                 QFixed kashidaWidth;
 
                 QGlyphLayout g;
