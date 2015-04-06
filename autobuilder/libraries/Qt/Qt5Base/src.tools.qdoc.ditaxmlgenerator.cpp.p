--- src/tools/qdoc/ditaxmlgenerator.cpp.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/tools/qdoc/ditaxmlgenerator.cpp	2014-08-17 16:17:06.151967303 +0100
@@ -2851,7 +2851,7 @@
                 idx = i;
             }
         }
-        idx += QChar('A').unicode();
+        idx += RISCOS_DEREF_QCHAR(QChar('A').unicode());
         QChar common(idx);
         commonPrefix = common;
         commonPrefixLen = 1;
