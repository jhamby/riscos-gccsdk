--- src/ui/TabBarWidget.h
+++ src/ui/TabBarWidget.h
@@ -127,6 +127,8 @@ protected slots:
 	void updatePinnedTabsAmount();
 	void updateStyle();
 	void setArea(Qt::ToolBarArea area);
+	void tabBarRequireModifier(bool isEnabled);
+	void tabBarEnableThumbnails(bool areEnabled);
 
 private:
 	PreviewWidget *m_previewWidget;


