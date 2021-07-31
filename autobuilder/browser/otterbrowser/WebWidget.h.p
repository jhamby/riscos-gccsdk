--- src/ui/WebWidget.h
+++ src/ui/WebWidget.h
@@ -304,6 +304,7 @@ protected slots:
 	void notifyUndoActionStateChanged();
 	void notifyPageActionsChanged();
 	void setStatusMessage(const QString &message, bool override = false);
+	void setFinished(int result, bool isChecked);
 
 private:
 	ContentsWidget *m_parent;


