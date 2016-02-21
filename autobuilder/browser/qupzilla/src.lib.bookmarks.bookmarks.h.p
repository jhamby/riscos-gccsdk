--- src/lib/bookmarks/bookmarks.h.orig	2016-02-21 12:03:42.434838240 +0000
+++ src/lib/bookmarks/bookmarks.h	2016-02-21 12:03:51.846838249 +0000
@@ -39,6 +39,7 @@
     void loadSettings();
 
     bool showOnlyIconsInToolbar() const;
+    bool showOnlyTextInToolbar() const;
 
     BookmarkItem* rootItem() const;
     BookmarkItem* toolbarFolder() const;
@@ -65,6 +66,7 @@
 
 public slots:
     void setShowOnlyIconsInToolbar(bool state);
+    void setShowOnlyTextInToolbar(bool state);
 
 signals:
     // Item was added to bookmarks
@@ -75,6 +77,7 @@
     void bookmarkChanged(BookmarkItem* item);
 
     void showOnlyIconsInToolbarChanged(bool show);
+    void showOnlyTextInToolbarChanged(bool show);
 
 private slots:
     void saveSettings();
@@ -102,6 +105,7 @@
     AutoSaver* m_autoSaver;
 
     bool m_showOnlyIconsInToolbar;
+    bool m_showOnlyTextInToolbar;
 };
 
 #endif // BOOKMARKS_H
