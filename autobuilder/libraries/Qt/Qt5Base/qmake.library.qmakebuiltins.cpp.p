--- qmake/library/qmakebuiltins.cpp.orig	2014-05-15 18:12:10.000000000 +0100
+++ qmake/library/qmakebuiltins.cpp	2014-05-27 19:55:15.910522836 +0100
@@ -236,7 +236,7 @@
     bool escaping = false;
     for (int i = 0, l = val.size(); i < l; i++) {
         QChar c = chars[i];
-        ushort uc = c.unicode();
+        ushort uc = RISCOS_DEREF_QCHAR(c.unicode());
         if (uc < 32) {
             if (!escaping) {
                 escaping = true;
