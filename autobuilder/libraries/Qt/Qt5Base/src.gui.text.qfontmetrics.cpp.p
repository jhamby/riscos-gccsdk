--- src/gui/text/qfontmetrics.cpp.orig	2014-05-15 18:12:15.000000000 +0100
+++ src/gui/text/qfontmetrics.cpp	2014-06-12 20:23:48.853654405 +0100
@@ -410,7 +410,7 @@
 */
 bool QFontMetrics::inFont(QChar ch) const
 {
-    return inFontUcs4(ch.unicode());
+    return inFontUcs4(RISCOS_DEREF_QCHAR(ch.unicode()));
 }
 
 /*!
@@ -453,7 +453,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
 
     qreal lb;
     engine->getGlyphBearings(glyph, &lb);
@@ -486,7 +486,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
 
     qreal rb;
     engine->getGlyphBearings(glyph, 0, &rb);
@@ -569,7 +569,7 @@
 */
 int QFontMetrics::width(QChar ch) const
 {
-    if (QChar::category(ch.unicode()) == QChar::Mark_NonSpacing)
+    if (QChar::category(RISCOS_DEREF_QCHAR(ch.unicode())) == QChar::Mark_NonSpacing)
         return 0;
 
     const int script = ch.script();
@@ -582,7 +582,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
     QFixed advance;
 
     QGlyphLayout glyphs;
@@ -633,7 +633,7 @@
 
         d->alterCharForCapitalization(ch);
 
-        glyph_t glyph = engine->glyphIndex(ch.unicode());
+        glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
         QFixed advance;
 
         QGlyphLayout glyphs;
@@ -708,7 +708,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
 
     glyph_metrics_t gm = engine->boundingBox(glyph);
     return QRect(qRound(gm.x), qRound(gm.y), qRound(gm.width), qRound(gm.height));
@@ -1275,7 +1275,7 @@
 */
 bool QFontMetricsF::inFont(QChar ch) const
 {
-    return inFontUcs4(ch.unicode());
+    return inFontUcs4(RISCOS_DEREF_QCHAR(ch.unicode()));
 }
 
 /*!
@@ -1320,7 +1320,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
 
     qreal lb;
     engine->getGlyphBearings(glyph, &lb);
@@ -1353,7 +1353,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
 
     qreal rb;
     engine->getGlyphBearings(glyph, 0, &rb);
@@ -1421,7 +1421,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
     QFixed advance;
 
     QGlyphLayout glyphs;
@@ -1492,7 +1492,7 @@
 
     d->alterCharForCapitalization(ch);
 
-    glyph_t glyph = engine->glyphIndex(ch.unicode());
+    glyph_t glyph = engine->glyphIndex(RISCOS_DEREF_QCHAR(ch.unicode()));
 
     glyph_metrics_t gm = engine->boundingBox(glyph);
     return QRectF(gm.x.toReal(), gm.y.toReal(), gm.width.toReal(), gm.height.toReal());
