--- src/corelib/tools/qchar.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qchar.cpp	2014-05-27 19:55:16.342522841 +0100
@@ -1523,6 +1523,13 @@
     return toCaseFolded_helper<ushort>(ch);
 }
 
+#ifdef Q_OS_RISCOS
+static inline ushort foldCase(ushort *ch)
+{
+    return toCaseFolded_helper<ushort>(*ch);
+}
+#endif
+
 /*!
     \fn QChar QChar::toCaseFolded() const
 
@@ -1607,7 +1614,7 @@
 */
 QDataStream &operator<<(QDataStream &out, QChar chr)
 {
-    out << quint16(chr.unicode());
+    out << quint16(RISCOS_DEREF_QCHAR(chr.unicode()));
     return out;
 }
 
@@ -1622,7 +1629,7 @@
 {
     quint16 u;
     in >> u;
-    chr.unicode() = ushort(u);
+    RISCOS_DEREF_QCHAR(chr.unicode()) = ushort(u);
     return in;
 }
 #endif // QT_NO_DATASTREAM
