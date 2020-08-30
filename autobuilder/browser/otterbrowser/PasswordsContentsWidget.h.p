--- src/modules/windows/passwords/PasswordsContentsWidget.h
+++ src/modules/windows/passwords/PasswordsContentsWidget.h
@@ -73,6 +73,7 @@ protected slots:
 	void removeHostPasswords();
 	void removeAllPasswords();
 	void showContextMenu(const QPoint &position);
+	void setActionStateChanged();
 
 private:
 	QStandardItemModel *m_model;


