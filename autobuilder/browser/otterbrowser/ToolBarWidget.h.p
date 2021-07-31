--- src/ui/ToolBarWidget.h
+++ src/ui/ToolBarWidget.h
@@ -116,6 +116,10 @@ protected slots:
 	void handleBookmarkRemoved(BookmarksItem *bookmark, BookmarksItem *previousParent);
 	void updateVisibility();
 	void setToolBarLocked(bool locked);
+	void requireModifier(bool isEnabled);
+	void enableThumbnails(bool areEnabled);
+	void statusBarSetFixedHeight(int iconSize);
+
 
 private:
 	MainWindow *m_mainWindow;


