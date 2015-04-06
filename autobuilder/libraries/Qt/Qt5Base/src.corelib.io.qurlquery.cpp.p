--- src/corelib/io/qurlquery.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qurlquery.cpp	2014-06-12 19:59:20.049635371 +0100
@@ -274,8 +274,8 @@
 void QUrlQueryPrivate::setQuery(const QString &query)
 {
     ushort prettyDecodedActions[] = {
-        decode(pairDelimiter.unicode()),
-        decode(valueDelimiter.unicode()),
+        decode(RISCOS_DEREF_QCHAR(pairDelimiter.unicode())),
+        decode(RISCOS_DEREF_QCHAR(valueDelimiter.unicode())),
         decode('#'),
         0
     };
@@ -562,8 +562,8 @@
 */
 void QUrlQuery::setQueryDelimiters(QChar valueDelimiter, QChar pairDelimiter)
 {
-    d->valueDelimiter = valueDelimiter.unicode();
-    d->pairDelimiter = pairDelimiter.unicode();
+    d->valueDelimiter = RISCOS_DEREF_QCHAR(valueDelimiter.unicode());
+    d->pairDelimiter = RISCOS_DEREF_QCHAR(pairDelimiter.unicode());
 }
 
 /*!
