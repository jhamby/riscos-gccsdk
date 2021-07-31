--- src/lib/bookmarks/bookmarks.cpp.orig	2016-02-21 11:59:35.778837988 +0000
+++ src/lib/bookmarks/bookmarks.cpp	2016-02-21 12:00:17.790838031 +0000
@@ -23,9 +23,9 @@
 #include "datapaths.h"
 #include "settings.h"
 #include "qztools.h"
-#include "json.h"
 
 #include <QFile>
+#include <QJsonDocument>
 
 Bookmarks::Bookmarks(QObject* parent)
     : QObject(parent)
@@ -49,6 +49,7 @@
     Settings settings;
     settings.beginGroup("Bookmarks");
     m_showOnlyIconsInToolbar = settings.value("showOnlyIconsInToolbar", false).toBool();
+    m_showOnlyTextInToolbar = settings.value("showOnlyTextInToolbar", false).toBool();
     settings.endGroup();
 }
 
@@ -57,6 +58,11 @@
     return m_showOnlyIconsInToolbar;
 }
 
+bool Bookmarks::showOnlyTextInToolbar() const
+{
+    return m_showOnlyTextInToolbar;
+}
+
 BookmarkItem* Bookmarks::rootItem() const
 {
     return m_root;
@@ -174,11 +180,18 @@
     emit showOnlyIconsInToolbarChanged(state);
 }
 
+void Bookmarks::setShowOnlyTextInToolbar(bool state)
+{
+    m_showOnlyTextInToolbar = state;
+    emit showOnlyTextInToolbarChanged(state);
+}
+
 void Bookmarks::saveSettings()
 {
     Settings settings;
     settings.beginGroup("Bookmarks");
     settings.setValue("showOnlyIconsInToolbar", m_showOnlyIconsInToolbar);
+    settings.setValue("showOnlyTextInToolbar", m_showOnlyTextInToolbar);
     settings.endGroup();
 
     saveBookmarks();
@@ -218,15 +231,11 @@
     const QString bookmarksFile = DataPaths::currentProfilePath() + QLatin1String("/bookmarks.json");
     const QString backupFile = bookmarksFile + QLatin1String(".old");
 
-    QFile file(bookmarksFile);
-    file.open(QFile::ReadOnly);
-    QByteArray data = file.readAll();
-    file.close();
-
-    Json json;
-    const QVariant res = json.parse(QString::fromUtf8(data));
+    QJsonParseError err;
+    QJsonDocument json = QJsonDocument::fromJson(QzTools::readAllFileByteContents(bookmarksFile), &err);
+    const QVariant res = json.toVariant();
 
-    if (!json.ok() || res.type() != QVariant::Map) {
+    if (err.error != QJsonParseError::NoError || res.type() != QVariant::Map) {
         qWarning() << "Bookmarks::init() Error parsing bookmarks! Using default bookmarks!";
         qWarning() << "Bookmarks::init() Your bookmarks have been backed up in" << backupFile;
 
@@ -235,9 +244,10 @@
         QFile::copy(bookmarksFile, backupFile);
 
         // Load default bookmarks
-        const QVariant data = json.parse(QzTools::readAllFileContents(":data/bookmarks.json"));
+        json = QJsonDocument::fromJson(QzTools::readAllFileByteContents(QSL(":data/bookmarks.json")), &err);
+        const QVariant data = json.toVariant();
 
-        Q_ASSERT(json.ok());
+        Q_ASSERT(err.error == QJsonParseError::NoError);
         Q_ASSERT(data.type() == QVariant::Map);
 
         loadBookmarksFromMap(data.toMap().value("roots").toMap());
@@ -273,10 +283,10 @@
     map.insert("version", Qz::bookmarksVersion);
     map.insert("roots", bookmarksMap);
 
-    Json json;
-    const QString data = json.serialize(map);
+    const QJsonDocument json = QJsonDocument::fromVariant(map);
+    const QByteArray data = json.toJson();
 
-    if (!json.ok() || data.isEmpty()) {
+    if (data.isEmpty()) {
         qWarning() << "Bookmarks::saveBookmarks() Error serializing bookmarks!";
         return;
     }
@@ -287,7 +297,7 @@
         qWarning() << "Bookmarks::saveBookmarks() Error opening bookmarks file for writing!";
     }
 
-    file.write(data.toUtf8());
+    file.write(data);
     file.close();
 }
 
