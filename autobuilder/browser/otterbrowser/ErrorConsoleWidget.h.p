--- src/modules/widgets/errorConsole/ErrorConsoleWidget.h
+++ src/modules/widgets/errorConsole/ErrorConsoleWidget.h
@@ -72,6 +72,7 @@ protected slots:
 	void filterCategories();
 	void filterMessages(const QString &filter);
 	void showContextMenu(const QPoint position);
+	void setTriggerAction();
 
 private:
 	QStandardItemModel *m_model;


