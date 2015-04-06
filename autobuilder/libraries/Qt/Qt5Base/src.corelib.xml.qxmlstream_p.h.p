--- src/corelib/xml/qxmlstream_p.h.orig	2013-08-25 19:03:36.000000000 +0100
+++ src/corelib/xml/qxmlstream_p.h	2013-11-17 16:49:32.672783781 +0000
@@ -958,7 +958,7 @@
     inline uint getChar();
     inline uint peekChar();
     inline void putChar(uint c) { putStack.push() = c; }
-    inline void putChar(QChar c) { putStack.push() =  c.unicode(); }
+    inline void putChar(QChar c) { putStack.push() =  RISCOS_DEREF_QCHAR(c.unicode()); }
     void putString(const QString &s, int from = 0);
     void putStringLiteral(const QString &s);
     void putReplacement(const QString &s);
@@ -1716,7 +1716,7 @@
                     bool wasSpace = true;
                     for (int i = 0; i < attribute.value.len; ++i) {
                         QChar c = textBuffer.at(attribute.value.pos + i);
-                        if (c.unicode() == ' ') {
+                        if (RISCOS_DEREF_QCHAR(c.unicode()) == ' ') {
                             if (wasSpace)
                                 continue;
                             wasSpace = true;
