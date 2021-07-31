--- src/ui/Window.h
+++ src/ui/Window.h
@@ -112,6 +112,7 @@ protected slots:
 	void handleGeometryChangeRequest(const QRect &geometry);
 	void notifyRequestedCloseWindow();
 	void updateNavigationBar();
+	void setWindowStateChanged();
 
 private:
 	MainWindow *m_mainWindow;


