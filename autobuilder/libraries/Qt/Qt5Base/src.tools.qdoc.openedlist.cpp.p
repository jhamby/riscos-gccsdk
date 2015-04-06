--- src/tools/qdoc/openedlist.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/openedlist.cpp	2014-08-17 16:33:42.339995872 +0100
@@ -148,7 +148,7 @@
     int u;
 
     for ( int i = 0; i < (int) str.length(); i++ ) {
-        u = str[i].toLower().unicode();
+        u = RISCOS_DEREF_QCHAR(str[i].toLower().unicode());
         if ( u >= 'a' && u <= 'z' ) {
             n *= 26;
             n += u - 'a' + 1;
