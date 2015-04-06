--- src/corelib/tools/qchar.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qchar.h	2014-05-27 19:55:16.346522842 +0100
@@ -46,6 +46,17 @@
 
 QT_BEGIN_NAMESPACE
 
+#ifdef Q_OS_RISCOS
+#define RISCOS_DEREF_QCHAR(ch) *ch
+/*static inline ushort RISCOS_DEREF_QCHAR (ushort *p) {
+    return *p;
+}*/
+#else
+/*static inline ushort& RISCOS_DEREF_QCHAR (ushort &p) {
+    return p;
+}*/
+#define RISCOS_DEREF_QCHAR(ch) ch
+#endif
 
 class QString;
 
@@ -388,7 +399,11 @@
 #endif
     inline char toLatin1() const;
     inline ushort unicode() const { return ucs; }
+#ifdef Q_OS_RISCOS
+    inline ushort *unicode() { return &ucs; }
+#else
     inline ushort &unicode() { return ucs; }
+#endif
 
 #if QT_DEPRECATED_SINCE(5, 0)
     QT_DEPRECATED static inline QChar fromAscii(char c)
@@ -440,7 +455,7 @@
         return (uint(high)<<10) + low - 0x35fdc00;
     }
     static inline uint surrogateToUcs4(QChar high, QChar low) {
-        return surrogateToUcs4(high.unicode(), low.unicode());
+        return surrogateToUcs4(RISCOS_DEREF_QCHAR(high.unicode()), RISCOS_DEREF_QCHAR(low.unicode()));
     }
     static inline ushort highSurrogate(uint ucs4) {
         return ushort((ucs4>>10) + 0xd7c0);
@@ -499,7 +514,11 @@
     QChar(uchar c);
 #endif
     ushort ucs;
-};
+}
+#ifdef Q_OS_RISCOS
+__attribute__((packed))
+#endif
+;
 
 Q_DECLARE_TYPEINFO(QChar, Q_MOVABLE_TYPE);
 
@@ -539,12 +558,12 @@
 inline bool QChar::isTitleCase(uint ucs4)
 { return ucs4 > 127 && QChar::category(ucs4) == Letter_Titlecase; }
 
-inline bool operator==(QChar c1, QChar c2) { return c1.unicode() == c2.unicode(); }
-inline bool operator!=(QChar c1, QChar c2) { return c1.unicode() != c2.unicode(); }
-inline bool operator<=(QChar c1, QChar c2) { return c1.unicode() <= c2.unicode(); }
-inline bool operator>=(QChar c1, QChar c2) { return c1.unicode() >= c2.unicode(); }
-inline bool operator<(QChar c1, QChar c2) { return c1.unicode() < c2.unicode(); }
-inline bool operator>(QChar c1, QChar c2) { return c1.unicode() > c2.unicode(); }
+inline bool operator==(QChar c1, QChar c2) { return RISCOS_DEREF_QCHAR(c1.unicode()) == RISCOS_DEREF_QCHAR(c2.unicode()); }
+inline bool operator!=(QChar c1, QChar c2) { return RISCOS_DEREF_QCHAR(c1.unicode()) != RISCOS_DEREF_QCHAR(c2.unicode()); }
+inline bool operator<=(QChar c1, QChar c2) { return RISCOS_DEREF_QCHAR(c1.unicode()) <= RISCOS_DEREF_QCHAR(c2.unicode()); }
+inline bool operator>=(QChar c1, QChar c2) { return RISCOS_DEREF_QCHAR(c1.unicode()) >= RISCOS_DEREF_QCHAR(c2.unicode()); }
+inline bool operator<(QChar c1, QChar c2) { return RISCOS_DEREF_QCHAR(c1.unicode()) < RISCOS_DEREF_QCHAR(c2.unicode()); }
+inline bool operator>(QChar c1, QChar c2) { return RISCOS_DEREF_QCHAR(c1.unicode()) > RISCOS_DEREF_QCHAR(c2.unicode()); }
 
 #ifndef QT_NO_DATASTREAM
 Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, QChar);
