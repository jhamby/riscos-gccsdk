--- src/tools/qdoc/doc.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/doc.cpp	2014-08-17 16:20:07.879972515 +0100
@@ -596,7 +596,7 @@
     while (pos < len) {
         QChar ch = in.at(pos);
 
-        switch (ch.unicode()) {
+        switch (RISCOS_DEREF_QCHAR(ch.unicode())) {
         case '\\':
         {
             QString cmdStr;
