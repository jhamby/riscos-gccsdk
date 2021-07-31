--- src/ui/StatusBarWidget.h
+++ src/ui/StatusBarWidget.h
@@ -37,6 +37,9 @@ protected:
 	void paintEvent(QPaintEvent *event) override;
 	void resizeEvent(QResizeEvent *event) override;
 
+protected slots:
+	void statusBarSetFixedHeight(int iconSize);
+
 private:
 	ToolBarWidget *m_toolBar;
 };


