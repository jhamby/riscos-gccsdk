--- src/tools/qdoc/config.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/config.cpp	2014-08-17 16:09:54.971954938 +0100
@@ -954,7 +954,7 @@
     location.advance(c); \
     ++i; \
     c = text.at(i); \
-    cc = c.unicode(); \
+    cc = RISCOS_DEREF_QCHAR(c.unicode()); \
 } while (0)
 
 #define SKIP_SPACES() \
@@ -992,7 +992,7 @@
 
     int i = 0;
     QChar c = text.at(0);
-    uint cc = c.unicode();
+    uint cc = RISCOS_DEREF_QCHAR(c.unicode());
     while (i < (int) text.length()) {
         if (cc == 0) {
             ++i;
