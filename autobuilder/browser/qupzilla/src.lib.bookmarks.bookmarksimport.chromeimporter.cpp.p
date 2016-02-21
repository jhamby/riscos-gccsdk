--- src/lib/bookmarks/bookmarksimport/chromeimporter.cpp.orig	2016-02-21 12:11:25.738838713 +0000
+++ src/lib/bookmarks/bookmarksimport/chromeimporter.cpp	2016-02-21 12:12:23.078838772 +0000
@@ -17,11 +17,11 @@
 * ============================================================ */
 #include "chromeimporter.h"
 #include "bookmarkitem.h"
-#include "json.h"
 
 #include <QDir>
 #include <QFileDialog>
 #include <QVariantList>
+#include <QJsonDocument>
 
 ChromeImporter::ChromeImporter(QObject* parent)
     : BookmarksImporter(parent)
@@ -63,13 +63,14 @@
 
 BookmarkItem* ChromeImporter::importBookmarks()
 {
-    QString bookmarks = QString::fromUtf8(m_file.readAll());
+    const QByteArray data = m_file.readAll();
     m_file.close();
 
-    Json json;
-    const QVariant res = json.parse(bookmarks);
+    QJsonParseError err;
+    QJsonDocument json = QJsonDocument::fromJson(data);
+    const QVariant res = json.toVariant();
 
-    if (!json.ok() || res.type() != QVariant::Map) {
+    if (err.error != QJsonParseError::NoError || res.type() != QVariant::Map) {
         setError(BookmarksImporter::tr("Cannot parse JSON file!"));
         return 0;
     }
