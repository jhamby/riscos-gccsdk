--- src/ui/ContentsDialog.h
+++ src/ui/ContentsDialog.h
@@ -50,6 +50,7 @@ protected:
 
 protected slots:
 	void handleButtonClick(QAbstractButton *button);
+	void setFinished(int result);
 
 private:
 	QBoxLayout *m_contentsLayout;


