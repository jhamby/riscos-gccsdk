--- src/tools/qdoc/codemarker.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/codemarker.cpp	2014-08-17 14:56:45.659829061 +0100
@@ -229,7 +229,7 @@
             }
             pendingWord.clear();
 
-            switch (ch.unicode()) {
+            switch (RISCOS_DEREF_QCHAR(ch.unicode())) {
             case '\0':
                 break;
             case '&':
