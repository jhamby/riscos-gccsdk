--- src/tools/qdoc/cppcodemarker.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/cppcodemarker.cpp	2014-08-17 15:17:16.731864365 +0100
@@ -910,7 +910,7 @@
             } while (ch.isLetterOrNumber() || ch == '.');
             tag = QStringLiteral("number");
         } else {
-            switch (ch.unicode()) {
+            switch (RISCOS_DEREF_QCHAR(ch.unicode())) {
             case '+':
             case '-':
             case '!':
