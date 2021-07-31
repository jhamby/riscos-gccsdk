--- src/ui/MenuBarWidget.cpp
+++ src/ui/MenuBarWidget.cpp
@@ -61,6 +61,7 @@ MenuBarWidget::MenuBarWidget(MainWindow *parent) : QMenuBar(parent),
 	{
 		reload();
 
+#ifndef __riscos__
 		connect(ToolBarsManager::getInstance(), &ToolBarsManager::toolBarModified, [&](int identifier)
 		{
 			if (identifier == ToolBarsManager::MenuBar)
@@ -68,6 +69,10 @@ MenuBarWidget::MenuBarWidget(MainWindow *parent) : QMenuBar(parent),
 				reload();
 			}
 		});
+#else
+		connect(ToolBarsManager::getInstance(), SIGNAL(toolBarModified(int)), this, SLOT(setReload(int)));
+#endif
+
 	}
 }
 
@@ -205,6 +210,17 @@ void MenuBarWidget::reload()
 	QTimer::singleShot(100, this, SLOT(updateGeometries()));
 }
 
+#ifdef __riscos__
+void MenuBarWidget::setReload(int identifier)
+{
+	if (identifier == ToolBarsManager::MenuBar)
+	{
+		reload();
+	}
+
+}
+#endif
+
 void MenuBarWidget::updateGeometries()
 {
 	if (!m_leftToolBar && !m_rightToolBar)


