--- src/lib/opensearch/opensearchengine.cpp.orig	2016-02-21 12:06:48.698838430 +0000
+++ src/lib/opensearch/opensearchengine.cpp	2016-02-21 12:07:09.426838451 +0000
@@ -37,7 +37,6 @@
 #include "opensearchengine.h"
 #include "qzregexp.h"
 #include "opensearchenginedelegate.h"
-#include "json.h"
 
 #include <qbuffer.h>
 #include <qcoreapplication.h>
@@ -47,9 +46,8 @@
 #include <qregexp.h>
 #include <qstringlist.h>
 
-#if QT_VERSION >= 0x050000
 #include <QUrlQuery>
-#endif
+#include <QJsonDocument>
 
 
 /*!
@@ -226,22 +224,14 @@
 
     QUrl retVal = QUrl::fromEncoded(parseTemplate(searchTerm, m_searchUrlTemplate).toUtf8());
 
-#if QT_VERSION >= 0x050000
     QUrlQuery query(retVal);
-#endif
     if (m_searchMethod != QLatin1String("post")) {
         Parameters::const_iterator end = m_searchParameters.constEnd();
         Parameters::const_iterator i = m_searchParameters.constBegin();
         for (; i != end; ++i) {
-#if QT_VERSION >= 0x050000
             query.addQueryItem(i->first, parseTemplate(searchTerm, i->second));
-#else
-            retVal.addQueryItem(i->first, parseTemplate(searchTerm, i->second));
-#endif
         }
-#if QT_VERSION >= 0x050000
         retVal.setQuery(query);
-#endif
     }
 
     return retVal;
@@ -255,21 +245,13 @@
 
     QUrl retVal = QUrl("http://foo.bar");
 
-#if QT_VERSION >= 0x050000
     QUrlQuery query(retVal);
-#endif
     Parameters::const_iterator end = m_searchParameters.constEnd();
     Parameters::const_iterator i = m_searchParameters.constBegin();
     for (; i != end; ++i) {
-#if QT_VERSION >= 0x050000
         query.addQueryItem(i->first, parseTemplate(searchTerm, i->second));
-#else
-        retVal.addQueryItem(i->first, parseTemplate(searchTerm, i->second));
-#endif
     }
-#if QT_VERSION >= 0x050000
     retVal.setQuery(query);
-#endif
 
     QByteArray data = retVal.toEncoded(QUrl::RemoveScheme);
     return data.contains('?') ? data.mid(data.lastIndexOf('?') + 1) : QByteArray();
@@ -324,22 +306,14 @@
 
     QUrl retVal = QUrl::fromEncoded(parseTemplate(searchTerm, m_suggestionsUrlTemplate).toUtf8());
 
-#if QT_VERSION >= 0x050000
     QUrlQuery query(retVal);
-#endif
     if (m_suggestionsMethod != QLatin1String("post")) {
         Parameters::const_iterator end = m_suggestionsParameters.constEnd();
         Parameters::const_iterator i = m_suggestionsParameters.constBegin();
         for (; i != end; ++i) {
-#if QT_VERSION >= 0x050000
             query.addQueryItem(i->first, parseTemplate(searchTerm, i->second));
-#else
-            retVal.addQueryItem(i->first, parseTemplate(searchTerm, i->second));
-#endif
         }
-#if QT_VERSION >= 0x050000
         retVal.setQuery(query);
-#endif
     }
 
     return retVal;
@@ -643,27 +617,27 @@
 
 void OpenSearchEngine::suggestionsObtained()
 {
-    QString response(QString::fromUtf8(m_suggestionsReply->readAll()));
-    response = response.trimmed();
+    const QByteArray response = m_suggestionsReply->readAll();
 
     m_suggestionsReply->close();
     m_suggestionsReply->deleteLater();
     m_suggestionsReply = 0;
 
-    Json json;
-    const QVariant res = json.parse(response);
+    QJsonParseError err;
+    QJsonDocument json = QJsonDocument::fromJson(response);
+    const QVariant res = json.toVariant();
 
-    if (!json.ok() || res.type() != QVariant::Map)
+    if (err.error != QJsonParseError::NoError || res.type() != QVariant::List)
         return;
 
-    const QVariantList list = res.toMap().value(QSL("1")).toList();
+    const QVariantList list = res.toList();
 
-    if (list.isEmpty())
+    if (list.size() < 2)
         return;
 
     QStringList out;
 
-    foreach (const QVariant &v, list)
+    foreach (const QVariant &v, list.at(1).toList())
         out.append(v.toString());
 
     emit suggestions(out);
