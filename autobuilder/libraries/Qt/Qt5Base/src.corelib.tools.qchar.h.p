--- src/corelib/tools/qchar.h.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/tools/qchar.h	2015-04-16 12:20:39.709451517 +0100
@@ -381,7 +381,11 @@
 #endif
     inline char toLatin1() const;
     Q_DECL_CONSTEXPR inline ushort unicode() const { return ucs; }
+#ifdef Q_OS_RISCOS
+    inline ushort &unicode() { ushort *ptr = &ucs; return *ptr; }
+#else
     inline ushort &unicode() { return ucs; }
+#endif
 
 #if QT_DEPRECATED_SINCE(5, 0)
     QT_DEPRECATED static inline QChar fromAscii(char c)
@@ -492,7 +496,11 @@
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
 
