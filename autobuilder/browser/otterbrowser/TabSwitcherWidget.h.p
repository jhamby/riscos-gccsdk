--- src/ui/TabSwitcherWidget.h
+++ src/ui/TabSwitcherWidget.h
@@ -73,6 +73,7 @@ protected slots:
 	void setTitle(const QString &title);
 	void setIcon(const QIcon &icon);
 	void setLoadingState(WebWidget::LoadingState state);
+	void setTabSwitcherPixmap();
 
 private:
 	MainWindow *m_mainWindow;


