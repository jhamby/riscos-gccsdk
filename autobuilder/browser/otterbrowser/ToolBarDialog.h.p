--- src/ui/ToolBarDialog.h
+++ src/ui/ToolBarDialog.h
@@ -73,6 +73,7 @@ protected slots:
 	void addBookmark(QAction *action);
 	void restoreDefaults();
 	void updateActions();
+	void dialogDefaultValue(OptionEntry textEntry, OptionEntry optionNameEntry);
 
 private:
 	ToolBarsManager::ToolBarDefinition m_definition;


