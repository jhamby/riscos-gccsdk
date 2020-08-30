--- src/ui/ToolBarWidget.cpp
+++ src/ui/ToolBarWidget.cpp
@@ -736,6 +736,7 @@ void ToolBarWidget::contextMenuEvent(QContextMenuEvent *event)
 	thumbnailsAction->setCheckable(true);
 	thumbnailsAction->setChecked(SettingsManager::getOption(SettingsManager::TabBar_EnableThumbnailsOption).toBool());
 
+#ifndef __riscos__
 	connect(cycleAction, &QAction::toggled, [&](bool isEnabled)
 	{
 		SettingsManager::setOption(SettingsManager::TabBar_RequireModifierToSwitchTabOnScrollOption, !isEnabled);
@@ -744,6 +745,10 @@ void ToolBarWidget::contextMenuEvent(QContextMenuEvent *event)
 	{
 		SettingsManager::setOption(SettingsManager::TabBar_EnableThumbnailsOption, areEnabled);
 	});
+#else
+	connect(cycleAction, SIGNAL(toggled(bool)), this, SLOT(requireModifier(bool)));
+	connect(thumbnailsAction, SIGNAL(toggled(bool)), this, SLOT(enableThumbnails(bool)));
+#endif
 
 	ActionExecutor::Object executor(m_mainWindow, m_mainWindow);
 	QMenu menu(this);
@@ -769,6 +774,23 @@ void ToolBarWidget::contextMenuEvent(QContextMenuEvent *event)
 	thumbnailsAction->deleteLater();
 }
 
+#ifdef __riscos__
+void ToolBarWidget::requireModifier(bool isEnabled)
+{
+	SettingsManager::setOption(SettingsManager::TabBar_RequireModifierToSwitchTabOnScrollOption, !isEnabled);
+}
+
+void ToolBarWidget::enableThumbnails(bool areEnabled)
+{
+	SettingsManager::setOption(SettingsManager::TabBar_EnableThumbnailsOption, areEnabled);
+}
+
+void ToolBarWidget::statusBarSetFixedHeight(int iconSize)
+{
+	setFixedHeight(iconSize);
+}
+#endif
+
 void ToolBarWidget::reload()
 {
 	const ToolBarsManager::ToolBarDefinition definition(getDefinition());


