--- src/ui/MainWindow.h
+++ src/ui/MainWindow.h
@@ -119,6 +119,7 @@ protected slots:
 	void setStatusMessage(const QString &message);
 	void updateWindowTitle();
 	void updateShortcuts();
+	void setAlert();
 	Window* openWindow(ContentsWidget *widget, SessionsManager::OpenHints hints = SessionsManager::DefaultOpen, int index = -1);
 
 private:


