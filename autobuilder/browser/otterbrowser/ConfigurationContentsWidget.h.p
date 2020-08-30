--- src/modules/windows/configuration/ConfigurationContentsWidget.h
+++ src/modules/windows/configuration/ConfigurationContentsWidget.h
@@ -87,6 +87,9 @@ protected slots:
 	void handleOptionChanged(int identifier, const QVariant &value);
 	void showContextMenu(const QPoint &position);
 	void updateActions();
+	void setButtonEnabled();
+	void trueSaveAll();
+	void falseSaveAll();
 
 private:
 	QStandardItemModel *m_model;


