--- src/ui/MenuBarWidget.h
+++ src/ui/MenuBarWidget.h
@@ -43,6 +43,7 @@ protected:
 
 protected slots:
 	void updateGeometries();
+	void setReload(int identifier);
 
 private:
 	MainWindow *m_mainWindow;


