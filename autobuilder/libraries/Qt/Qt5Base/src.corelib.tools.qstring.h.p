--- src/corelib/tools/qstring.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qstring.h	2015-04-02 18:40:57.552356514 +0100
@@ -49,7 +49,7 @@
 
 #include <string>
 
-#if defined(Q_OS_ANDROID)
+#if defined(Q_OS_ANDROID) || defined(Q_OS_RISCOS)
 // std::wstring is disabled on android's glibc, as bionic lacks certain features
 // that libstdc++ checks for (like mbcslen).
 namespace std
@@ -401,7 +401,7 @@
     inline QString &operator+=(QChar c) {
         if (d->ref.isShared() || uint(d->size) + 2u > d->alloc)
             reallocData(uint(d->size) + 2u, true);
-        d->data()[d->size++] = c.unicode();
+        d->data()[d->size++] = RISCOS_DEREF_QCHAR(c.unicode());
         d->data()[d->size] = '\0';
         return *this;
     }
@@ -911,7 +911,7 @@
     { return i < s.d->size ? s.d->data()[i] : 0; }
     inline QCharRef &operator=(QChar c)
     { if (i >= s.d->size) s.expand(i); else s.detach();
-      s.d->data()[i] = c.unicode(); return *this; }
+      s.d->data()[i] = RISCOS_DEREF_QCHAR(c.unicode()); return *this; }
 
     // An operator= for each QChar cast constructors
 #ifndef QT_NO_CAST_FROM_ASCII
@@ -980,9 +980,8 @@
     QT_DEPRECATED  char toAscii() const { return QChar(*this).toLatin1(); }
 #endif
     char toLatin1() const { return QChar(*this).toLatin1(); }
-    ushort unicode() const { return QChar(*this).unicode(); }
-    ushort& unicode() { return s.data()[i].unicode(); }
-
+    ushort unicode() const { return RISCOS_DEREF_QCHAR(QChar(*this).unicode()); }
+    ushort& unicode() { return RISCOS_DEREF_QCHAR(s.data()[i].unicode()); }
 };
 Q_DECLARE_TYPEINFO(QCharRef, Q_MOVABLE_TYPE);
 
