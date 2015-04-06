--- src/tools/qdoc/generator.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/generator.cpp	2014-08-17 16:24:34.159980151 +0100
@@ -386,7 +386,7 @@
     bool begun = false;
     for (int i = 0; i != base.size(); ++i) {
         QChar c = base.at(i);
-        uint u = c.unicode();
+        uint u = RISCOS_DEREF_QCHAR(c.unicode());
         if (u >= 'A' && u <= 'Z')
             u += 'a' - 'A';
         if ((u >= 'a' &&  u <= 'z') || (u >= '0' && u <= '9')) {
