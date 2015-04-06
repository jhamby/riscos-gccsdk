--- src/tools/qdoc/yyindent.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/yyindent.cpp	2014-08-17 16:39:59.248006681 +0100
@@ -558,7 +558,7 @@
             j--;
             QChar ch = (*yyLine)[j];
 
-            switch ( ch.unicode() ) {
+            switch ( RISCOS_DEREF_QCHAR(ch.unicode()) ) {
             case ')':
                 delimDepth++;
                 break;
@@ -706,7 +706,7 @@
             j--;
             QChar ch = (*yyLine)[j];
 
-            switch ( ch.unicode() ) {
+            switch ( RISCOS_DEREF_QCHAR(ch.unicode()) ) {
             case ')':
             case ']':
                 delimDepth++;
