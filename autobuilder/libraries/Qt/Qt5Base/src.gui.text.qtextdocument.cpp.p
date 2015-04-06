--- src/gui/text/qtextdocument.cpp.orig	2014-05-15 18:12:15.000000000 +0100
+++ src/gui/text/qtextdocument.cpp	2014-05-27 19:55:16.354522842 +0100
@@ -1160,7 +1160,7 @@
     QChar *e = uc + txt.size();
 
     for (; uc != e; ++uc) {
-        switch (uc->unicode()) {
+        switch (RISCOS_DEREF_QCHAR(uc->unicode())) {
         case 0xfdd0: // QTextBeginningOfFrame
         case 0xfdd1: // QTextEndOfFrame
         case QChar::ParagraphSeparator:
