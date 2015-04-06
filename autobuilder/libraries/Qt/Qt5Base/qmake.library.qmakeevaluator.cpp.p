--- qmake/library/qmakeevaluator.cpp.orig	2014-05-15 18:12:10.000000000 +0100
+++ qmake/library/qmakeevaluator.cpp	2014-05-27 19:55:16.378522842 +0100
@@ -2001,7 +2001,7 @@
     bool quote = forceQuote || val.isEmpty();
     for (int i = 0, l = val.size(); i < l; i++) {
         QChar c = chars[i];
-        ushort uc = c.unicode();
+        ushort uc = RISCOS_DEREF_QCHAR(c.unicode());
         if (uc < 32) {
             switch (uc) {
             case '\r':
