--- src/corelib/io/qtextstream.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qtextstream.cpp	2014-05-27 19:55:16.350522842 +0100
@@ -1659,7 +1659,7 @@
         // Parse digits
         int ndigits = 0;
         while (getChar(&dig)) {
-            int n = dig.toLower().unicode();
+            int n = RISCOS_DEREF_QCHAR(dig.toLower().unicode());
             if (n == '0' || n == '1') {
                 val <<= 1;
                 val += n - '0';
@@ -1685,7 +1685,7 @@
         // Parse digits
         int ndigits = 0;
         while (getChar(&dig)) {
-            int n = dig.toLower().unicode();
+            int n = RISCOS_DEREF_QCHAR(dig.toLower().unicode());
             if (n >= '0' && n <= '7') {
                 val *= 8;
                 val += n - '0';
@@ -1750,7 +1750,7 @@
         // Parse digits
         int ndigits = 0;
         while (getChar(&dig)) {
-            int n = dig.toLower().unicode();
+            int n = RISCOS_DEREF_QCHAR(dig.toLower().unicode());
             if (n >= '0' && n <= '9') {
                 val <<= 4;
                 val += n - '0';
@@ -1845,7 +1845,7 @@
 
     QChar c;
     while (getChar(&c)) {
-        switch (c.unicode()) {
+        switch (RISCOS_DEREF_QCHAR(c.unicode())) {
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
             input = InputDigit;
