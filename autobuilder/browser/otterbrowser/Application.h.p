--- src/core/Application.h
+++ src/core/Application.h
@@ -98,6 +98,7 @@ protected slots:
 	void handleFocusObjectChanged(QObject *object);
 	void handleUpdateCheckResult(const QVector<UpdateChecker::UpdateInformation> &availableUpdates);
 	void showUpdateDetails();
+	void setActiveWindow(MainWindow *window);
 
 private:
 	static Application *m_instance;


