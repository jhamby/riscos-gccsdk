--- src/corelib/tools/qstring.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qstring.cpp	2014-05-27 19:55:16.362522842 +0100
@@ -647,7 +647,7 @@
     Qt::CaseSensitivity cs)
 {
     const ushort *s = (const ushort *)str;
-    ushort c = ch.unicode();
+    ushort c = RISCOS_DEREF_QCHAR(ch.unicode());
     if (from < 0)
         from = qMax(from + len, 0);
     if (from < len) {
@@ -1443,7 +1443,7 @@
         d->data()[size] = '\0';
         ushort *i = d->data() + size;
         ushort *b = d->data();
-        const ushort value = ch.unicode();
+        const ushort value = RISCOS_DEREF_QCHAR(ch.unicode());
         while (i != b)
            *--i = value;
     }
@@ -1478,7 +1478,7 @@
     d = Data::allocate(2);
     Q_CHECK_PTR(d);
     d->size = 1;
-    d->data()[0] = ch.unicode();
+    d->data()[0] = RISCOS_DEREF_QCHAR(ch.unicode());
     d->data()[1] = '\0';
 }
 
@@ -1851,7 +1851,7 @@
         return *this;
     expand(qMax(i, d->size));
     ::memmove(d->data() + i + 1, d->data() + i, (d->size - i - 1) * sizeof(QChar));
-    d->data()[i] = ch.unicode();
+    d->data()[i] = RISCOS_DEREF_QCHAR(ch.unicode());
     return *this;
 }
 
@@ -1962,7 +1962,7 @@
 {
     if (d->ref.isShared() || uint(d->size) + 2u > d->alloc)
         reallocData(uint(d->size) + 2u, true);
-    d->data()[d->size++] = ch.unicode();
+    d->data()[d->size++] = RISCOS_DEREF_QCHAR(ch.unicode());
     d->data()[d->size] = '\0';
     return *this;
 }
@@ -2089,7 +2089,7 @@
 QString &QString::remove(QChar ch, Qt::CaseSensitivity cs)
 {
     int i = 0;
-    ushort c = ch.unicode();
+    ushort c = RISCOS_DEREF_QCHAR(ch.unicode());
     if (cs == Qt::CaseSensitive) {
         while (i < d->size)
             if (d->data()[i] == ch)
@@ -2351,7 +2351,7 @@
     if (d->size == 0)
         return *this;
 
-    ushort cc = (cs == Qt::CaseSensitive ? ch.unicode() : ch.toCaseFolded().unicode());
+    ushort cc = (cs == Qt::CaseSensitive ? RISCOS_DEREF_QCHAR(ch.unicode()) : RISCOS_DEREF_QCHAR(ch.toCaseFolded().unicode()));
 
     int index = 0;
     while (1) {
@@ -2393,8 +2393,8 @@
 */
 QString& QString::replace(QChar before, QChar after, Qt::CaseSensitivity cs)
 {
-    ushort a = after.unicode();
-    ushort b = before.unicode();
+    ushort a = RISCOS_DEREF_QCHAR(after.unicode());
+    ushort b = RISCOS_DEREF_QCHAR(before.unicode());
     if (d->size) {
         detach();
         ushort *i = d->data();
@@ -7031,7 +7031,7 @@
 
             if (field_width > 0) { // left padded
                 for (uint i = 0; i < pad_chars; ++i)
-                    (rc++)->unicode() = fillChar.unicode();
+                    RISCOS_DEREF_QCHAR((rc++)->unicode()) = RISCOS_DEREF_QCHAR(fillChar.unicode());
             }
 
             if (locale_arg) {
@@ -7045,7 +7045,7 @@
 
             if (field_width < 0) { // right padded
                 for (uint i = 0; i < pad_chars; ++i)
-                    (rc++)->unicode() = fillChar.unicode();
+                    RISCOS_DEREF_QCHAR((rc++)->unicode()) = RISCOS_DEREF_QCHAR(fillChar.unicode());
             }
 
             if (++repl_cnt == d.occurrences) {
@@ -9272,7 +9272,7 @@
 static inline int qt_last_index_of(const QChar *haystack, int haystackLen, QChar needle,
                                    int from, Qt::CaseSensitivity cs)
 {
-    ushort c = needle.unicode();
+    ushort c = RISCOS_DEREF_QCHAR(needle.unicode());
     if (from < 0)
         from += haystackLen;
     if (uint(from) >= uint(haystackLen))
@@ -9316,7 +9316,7 @@
 static inline int qt_string_count(const QChar *unicode, int size, QChar ch,
                                   Qt::CaseSensitivity cs)
 {
-    ushort c = ch.unicode();
+    ushort c = RISCOS_DEREF_QCHAR(ch.unicode());
     int num = 0;
     const ushort *b = reinterpret_cast<const ushort*>(unicode);
     const ushort *i = b + size;
