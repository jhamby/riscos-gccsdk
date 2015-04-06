--- src/xml/dom/qdom.cpp.orig	2013-08-25 19:03:31.000000000 +0100
+++ src/xml/dom/qdom.cpp	2013-11-17 16:59:28.800791506 +0000
@@ -650,7 +650,7 @@
     for (int i = 0; i < name.size(); ++i) {
         QChar c = name.at(i);
         if (firstChar) {
-            if (QXmlUtils::isLetter(c) || c.unicode() == '_' || c.unicode() == ':') {
+            if (QXmlUtils::isLetter(c) || RISCOS_DEREF_QCHAR(c.unicode()) == '_' || RISCOS_DEREF_QCHAR(c.unicode()) == ':') {
                 result.append(c);
                 firstChar = false;
             } else if (QDomImplementationPrivate::invalidDataPolicy == QDomImplementation::ReturnNullNode) {
