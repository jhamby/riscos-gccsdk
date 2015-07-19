--- src/corelib/tools/qchar.h.orig	2015-06-29 21:05:05.000000000 +0100
+++ src/corelib/tools/qchar.h	2015-07-15 18:24:19.946735897 +0100
@@ -409,7 +409,11 @@
 #endif
     Q_DECL_CONSTEXPR inline char toLatin1() const;
     Q_DECL_CONSTEXPR inline ushort unicode() const { return ucs; }
+#ifdef Q_OS_RISCOS
+    inline ushort &unicode() { ushort *ptr = &ucs; return *ptr; }
+#else
     inline ushort &unicode() { return ucs; }
+#endif
 
 #if QT_DEPRECATED_SINCE(5, 0)
     QT_DEPRECATED static Q_DECL_CONSTEXPR inline QChar fromAscii(char c)
@@ -532,7 +536,11 @@
     friend Q_DECL_CONSTEXPR bool operator==(QChar, QChar);
     friend Q_DECL_CONSTEXPR bool operator< (QChar, QChar);
     ushort ucs;
-};
+}
+#ifdef Q_OS_RISCOS
+__attribute__((packed))
+#endif
+;
 
 Q_DECLARE_TYPEINFO(QChar, Q_MOVABLE_TYPE);
 
