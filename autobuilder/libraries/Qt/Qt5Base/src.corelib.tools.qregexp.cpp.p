--- src/corelib/tools/qregexp.cpp.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/tools/qregexp.cpp	2014-05-27 19:55:16.374522842 +0100
@@ -695,6 +695,10 @@
 #endif
 
 const int NumBadChars = 64;
+#ifdef Q_OS_RISCOS
+/* P is for pointer, we expect a short * here.  */
+#define BadCharP(ch) (RISCOS_DEREF_QCHAR((ch).unicode()) % NumBadChars)
+#endif
 #define BadChar(ch) ((ch).unicode() % NumBadChars)
 
 const int NoOccurrence = INT_MAX;
@@ -1484,7 +1488,7 @@
 
 int QRegExpEngine::createState(QChar ch)
 {
-    return setupState(ch.unicode());
+    return setupState(RISCOS_DEREF_QCHAR(ch.unicode()));
 }
 
 int QRegExpEngine::createState(const QRegExpCharClass &cc)
@@ -2419,14 +2423,18 @@
 bool QRegExpCharClass::in(QChar ch) const
 {
 #ifndef QT_NO_REGEXP_OPTIM
+#ifdef Q_OS_RISCOS
+    if (occ1.at(BadCharP(ch)) == NoOccurrence)
+#else
     if (occ1.at(BadChar(ch)) == NoOccurrence)
+#endif
         return n;
 #endif
 
     if (c != 0 && (c & FLAG(ch.category())) != 0)
         return !n;
 
-    const int uc = ch.unicode();
+    const int uc = RISCOS_DEREF_QCHAR(ch.unicode());
     int size = r.size();
 
     for (int i = 0; i < size; ++i) {
@@ -2495,8 +2503,12 @@
     leftStr = ch;
     rightStr = ch;
     maxl = 1;
+#ifdef Q_OS_RISCOS
+    occ1[BadCharP(ch)] = 0;
+#else
     occ1[BadChar(ch)] = 0;
 #endif
+#endif
     minl = 1;
 }
 
@@ -3266,7 +3278,7 @@
     case 'x':
         val = 0;
         for (i = 0; i < 4; i++) {
-            low = QChar(yyCh).toLower().unicode();
+            low = RISCOS_DEREF_QCHAR(QChar(yyCh).toLower().unicode());
             if (low >= '0' && low <= '9')
                 val = (val << 4) | (low - '0');
             else if (low >= 'a' && low <= 'f')
