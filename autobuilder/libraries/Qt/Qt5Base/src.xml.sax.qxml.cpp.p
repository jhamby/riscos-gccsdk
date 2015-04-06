--- src/xml/sax/qxml.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/xml/sax/qxml.cpp	2014-05-27 19:55:16.378522842 +0100
@@ -1398,7 +1398,7 @@
     // is return EndOfDocument. We do *not* return EndOfData, because the reader
     // will then just call this function again to get the next char.
     QChar c = d->unicode[d->pos++];
-    if (c.unicode() == EndOfData)
+    if (RISCOS_DEREF_QCHAR(c.unicode()) == EndOfData)
         c = EndOfDocument;
     return c;
 }
@@ -2735,7 +2735,7 @@
 
 inline bool QXmlSimpleReaderPrivate::atEnd()
 {
-    return (c.unicode()|0x0001) == 0xffff;
+    return (RISCOS_DEREF_QCHAR(c.unicode())|0x0001) == 0xffff;
 }
 
 inline void QXmlSimpleReaderPrivate::stringClear()
@@ -3083,7 +3083,7 @@
 
 static inline bool is_S(QChar ch)
 {
-    ushort uc = ch.unicode();
+    ushort uc = RISCOS_DEREF_QCHAR(ch.unicode());
     return (uc == ' ' || uc == '\t' || uc == '\n' || uc == '\r');
 }
 
@@ -3123,7 +3123,7 @@
 
 static inline NameChar fastDetermineNameChar(QChar ch)
 {
-    ushort uc = ch.unicode();
+    ushort uc = RISCOS_DEREF_QCHAR(ch.unicode());
     if (!(uc & ~0x7f)) // uc < 128
         return (NameChar)nameCharTable[uc];
 
@@ -8030,7 +8030,7 @@
 
     // the following could be written nicer, but since it is a time-critical
     // function, rather optimize for speed
-    ushort uc = c.unicode();
+    ushort uc = RISCOS_DEREF_QCHAR(c.unicode());
     c = inputSource->next();
     // If we are not incremental parsing, we just skip over EndOfData chars to give the
     // parser an uninterrupted stream of document chars.
