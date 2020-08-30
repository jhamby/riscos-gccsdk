--- src/modules/backends/web/qtwebkit/QtWebKitWebWidget.h
+++ src/modules/backends/web/qtwebkit/QtWebKitWebWidget.h
@@ -173,6 +173,8 @@ protected slots:
 	void notifyContentStateChanged();
 	void updateAmountOfDeferredPlugins();
 	void updateOptions(const QUrl &url);
+	void setActionsStateChanged();
+	void setTriggerAction();
 
 private:
 	QWebView *m_webView;


