--- src/gui/text/qtextodfwriter.cpp.orig	2013-08-25 19:03:32.000000000 +0100
+++ src/gui/text/qtextodfwriter.cpp	2013-11-18 12:47:24.812515467 +0000
@@ -304,7 +304,7 @@
         int exportedIndex = 0;
         for (int i=0; i <= fragmentText.count(); ++i) {
             QChar character = fragmentText[i];
-            bool isSpace = character.unicode() == ' ';
+            bool isSpace = RISCOS_DEREF_QCHAR(character.unicode()) == ' ';
 
             // find more than one space. -> <text:s text:c="2" />
             if (!isSpace && escapeNextSpace && precedingSpaces > 1) {
@@ -320,13 +320,13 @@
             }
 
             if (i < fragmentText.count()) {
-                if (character.unicode() == 0x2028) { // soft-return
+                if (RISCOS_DEREF_QCHAR(character.unicode()) == 0x2028) { // soft-return
                     //if (exportedIndex < i)
                         writer.writeCharacters(fragmentText.mid(exportedIndex, i - exportedIndex));
                     writer.writeEmptyElement(textNS, QString::fromLatin1("line-break"));
                     exportedIndex = i+1;
                     continue;
-                } else if (character.unicode() == '\t') { // Tab
+                } else if (RISCOS_DEREF_QCHAR(character.unicode()) == '\t') { // Tab
                     //if (exportedIndex < i)
                         writer.writeCharacters(fragmentText.mid(exportedIndex, i - exportedIndex));
                     writer.writeEmptyElement(textNS, QString::fromLatin1("tab"));
