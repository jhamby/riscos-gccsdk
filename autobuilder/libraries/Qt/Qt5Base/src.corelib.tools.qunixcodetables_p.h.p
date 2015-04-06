--- src/corelib/tools/qunicodetables_p.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qunicodetables_p.h	2014-05-27 19:55:16.366522842 +0100
@@ -157,19 +157,19 @@
 
 Q_CORE_EXPORT GraphemeBreakClass QT_FASTCALL graphemeBreakClass(uint ucs4);
 inline GraphemeBreakClass graphemeBreakClass(QChar ch)
-{ return graphemeBreakClass(ch.unicode()); }
+{ return graphemeBreakClass(RISCOS_DEREF_QCHAR(ch.unicode())); }
 
 Q_CORE_EXPORT WordBreakClass QT_FASTCALL wordBreakClass(uint ucs4);
 inline WordBreakClass wordBreakClass(QChar ch)
-{ return wordBreakClass(ch.unicode()); }
+{ return wordBreakClass(RISCOS_DEREF_QCHAR(ch.unicode())); }
 
 Q_CORE_EXPORT SentenceBreakClass QT_FASTCALL sentenceBreakClass(uint ucs4);
 inline SentenceBreakClass sentenceBreakClass(QChar ch)
-{ return sentenceBreakClass(ch.unicode()); }
+{ return sentenceBreakClass(RISCOS_DEREF_QCHAR(ch.unicode())); }
 
 Q_CORE_EXPORT LineBreakClass QT_FASTCALL lineBreakClass(uint ucs4);
 inline LineBreakClass lineBreakClass(QChar ch)
-{ return lineBreakClass(ch.unicode()); }
+{ return lineBreakClass(RISCOS_DEREF_QCHAR(ch.unicode())); }
 
 } // namespace QUnicodeTables
 
