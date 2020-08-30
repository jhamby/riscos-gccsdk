--- src/ui/StatusBarWidget.cpp
+++ src/ui/StatusBarWidget.cpp
@@ -35,12 +35,23 @@ StatusBarWidget::StatusBarWidget(MainWindow *parent) : QStatusBar(parent),
 	setSizeGripEnabled(false);
 	setStyleSheet(QLatin1String("padding:1px;"));
 
+#ifndef __riscos__
 	connect(m_toolBar, &ToolBarWidget::iconSizeChanged, [&](int iconSize)
 	{
 		setFixedHeight(iconSize);
 	});
+#else
+	connect(m_toolBar, SIGNAL(iconSizeChanged(int)), this, SLOT(statusBarSetFixedHeight(int)));
+#endif
 }
 
+#ifdef __riscos__
+void StatusBarWidget::statusBarSetFixedHeight(int iconSize)
+{
+	setFixedHeight(iconSize);
+}
+#endif
+
 void StatusBarWidget::paintEvent(QPaintEvent *event)
 {
 	Q_UNUSED(event)


