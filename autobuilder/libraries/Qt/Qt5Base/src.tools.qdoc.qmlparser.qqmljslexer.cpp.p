--- src/tools/qdoc/qmlparser/qqmljslexer.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/qmlparser/qqmljslexer.cpp	2014-08-17 17:31:19.108095006 +0100
@@ -76,13 +76,13 @@
 
 static inline QChar convertHex(QChar c1, QChar c2)
 {
-    return QChar((convertHex(c1.unicode()) << 4) + convertHex(c2.unicode()));
+    return QChar((convertHex(RISCOS_DEREF_QCHAR(c1.unicode())) << 4) + convertHex(RISCOS_DEREF_QCHAR(c2.unicode())));
 }
 
 static inline QChar convertUnicode(QChar c1, QChar c2, QChar c3, QChar c4)
 {
-    return QChar((convertHex(c3.unicode()) << 4) + convertHex(c4.unicode()),
-                 (convertHex(c1.unicode()) << 4) + convertHex(c2.unicode()));
+    return QChar((convertHex(RISCOS_DEREF_QCHAR(c3.unicode())) << 4) + convertHex(RISCOS_DEREF_QCHAR(c4.unicode())),
+                 (convertHex(RISCOS_DEREF_QCHAR(c1.unicode())) << 4) + convertHex(RISCOS_DEREF_QCHAR(c2.unicode())));
 }
 
 Lexer::Lexer(Engine *engine)
@@ -355,8 +355,8 @@
 static inline bool isIdentifierStart(QChar ch)
 {
     // fast path for ascii
-    if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') ||
-        (ch.unicode() >= 'A' && ch.unicode() <= 'Z') ||
+    if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') ||
+        (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') ||
         ch == '$' || ch == '_')
         return true;
 
@@ -377,11 +377,11 @@
 static bool isIdentifierPart(QChar ch)
 {
     // fast path for ascii
-    if ((ch.unicode() >= 'a' && ch.unicode() <= 'z') ||
-        (ch.unicode() >= 'A' && ch.unicode() <= 'Z') ||
-        (ch.unicode() >= '0' && ch.unicode() <= '9') ||
+    if ((RISCOS_DEREF_QCHAR(ch.unicode()) >= 'a' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'z') ||
+        (RISCOS_DEREF_QCHAR(ch.unicode()) >= 'A' && RISCOS_DEREF_QCHAR(ch.unicode()) <= 'Z') ||
+        (RISCOS_DEREF_QCHAR(ch.unicode()) >= '0' && RISCOS_DEREF_QCHAR(ch.unicode()) <= '9') ||
         ch == '$' || ch == '_' ||
-        ch.unicode() == 0x200c /* ZWNJ */ || ch.unicode() == 0x200d /* ZWJ */)
+        RISCOS_DEREF_QCHAR(ch.unicode()) == 0x200c /* ZWNJ */ || RISCOS_DEREF_QCHAR(ch.unicode()) == 0x200d /* ZWJ */)
         return true;
 
     switch (ch.category()) {
@@ -564,7 +564,7 @@
             chars.append(ch.unicode()); // append the `.'
 
             while (_char.isDigit()) {
-                chars.append(_char.unicode());
+                chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                 scanChar();
             }
 
@@ -572,16 +572,16 @@
                 if (_codePtr[0].isDigit() || ((_codePtr[0] == QLatin1Char('+') || _codePtr[0] == QLatin1Char('-')) &&
                                               _codePtr[1].isDigit())) {
 
-                    chars.append(_char.unicode());
+                    chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                     scanChar(); // consume `e'
 
                     if (_char == QLatin1Char('+') || _char == QLatin1Char('-')) {
-                        chars.append(_char.unicode());
+                        chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                         scanChar(); // consume the sign
                     }
 
                     while (_char.isDigit()) {
-                        chars.append(_char.unicode());
+                        chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                         scanChar();
                     }
                 }
@@ -729,7 +729,7 @@
 
                 QChar u;
 
-                switch (_char.unicode()) {
+                switch (RISCOS_DEREF_QCHAR(_char.unicode())) {
                 // unicode escape sequence
                 case 'u': {
                     bool ok = false;
@@ -922,17 +922,17 @@
     }
 
     QVarLengthArray<char,32> chars;
-    chars.append(ch.unicode());
+    chars.append(RISCOS_DEREF_QCHAR(ch.unicode()));
 
     if (ch == QLatin1Char('0') && (_char == QLatin1Char('x') || _char == QLatin1Char('X'))) {
         ch = _char; // remember the x or X to use it in the error message below.
 
         // parse hex integer literal
-        chars.append(_char.unicode());
+        chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
         scanChar(); // consume `x'
 
         while (isHexDigit(_char)) {
-            chars.append(_char.unicode());
+            chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
             scanChar();
         }
 
@@ -948,16 +948,16 @@
 
     // decimal integer literal
     while (_char.isDigit()) {
-        chars.append(_char.unicode());
+        chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
         scanChar(); // consume the digit
     }
 
     if (_char == QLatin1Char('.')) {
-        chars.append(_char.unicode());
+        chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
         scanChar(); // consume `.'
 
         while (_char.isDigit()) {
-            chars.append(_char.unicode());
+            chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
             scanChar();
         }
 
@@ -965,16 +965,16 @@
             if (_codePtr[0].isDigit() || ((_codePtr[0] == QLatin1Char('+') || _codePtr[0] == QLatin1Char('-')) &&
                                           _codePtr[1].isDigit())) {
 
-                chars.append(_char.unicode());
+                chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                 scanChar(); // consume `e'
 
                 if (_char == QLatin1Char('+') || _char == QLatin1Char('-')) {
-                    chars.append(_char.unicode());
+                    chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                     scanChar(); // consume the sign
                 }
 
                 while (_char.isDigit()) {
-                    chars.append(_char.unicode());
+                    chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                     scanChar();
                 }
             }
@@ -983,16 +983,16 @@
         if (_codePtr[0].isDigit() || ((_codePtr[0] == QLatin1Char('+') || _codePtr[0] == QLatin1Char('-')) &&
                                       _codePtr[1].isDigit())) {
 
-            chars.append(_char.unicode());
+            chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
             scanChar(); // consume `e'
 
             if (_char == QLatin1Char('+') || _char == QLatin1Char('-')) {
-                chars.append(_char.unicode());
+                chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                 scanChar(); // consume the sign
             }
 
             while (_char.isDigit()) {
-                chars.append(_char.unicode());
+                chars.append(RISCOS_DEREF_QCHAR(_char.unicode()));
                 scanChar();
             }
         }
@@ -1031,7 +1031,7 @@
         _tokenText += QLatin1Char('=');
 
     while (true) {
-        switch (_char.unicode()) {
+        switch (RISCOS_DEREF_QCHAR(_char.unicode())) {
         case '/':
             scanChar();
 
@@ -1149,7 +1149,7 @@
             || ch == QLatin1Char('$')
             || ch == QLatin1Char('_'))
         return true;
-    if (ch.unicode() < 128)
+    if (RISCOS_DEREF_QCHAR(ch.unicode()) < 128)
         return false;
     return ch.isLetterOrNumber();
 }
