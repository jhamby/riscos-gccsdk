--- src/ui/TabBarWidget.cpp
+++ src/ui/TabBarWidget.cpp
@@ -541,9 +541,9 @@ TabBarWidget::TabBarWidget(QWidget *parent) : QTabBar(parent),
 		connect(toolBar, SIGNAL(areaChanged(Qt::ToolBarArea)), this, SLOT(setArea(Qt::ToolBarArea)));
 	}
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &TabBarWidget::handleOptionChanged);
-	connect(ThemesManager::getInstance(), &ThemesManager::widgetStyleChanged, this, &TabBarWidget::updateStyle);
-	connect(this, &TabBarWidget::currentChanged, this, &TabBarWidget::updatePreviewPosition);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
+	connect(ThemesManager::getInstance(), SIGNAL(widgetStyleChanged()), this, SLOT(updateStyle()));
+	connect(this, SIGNAL(currentChanged(int)), this, SLOT(updatePreviewPosition()));
 }
 
 void TabBarWidget::changeEvent(QEvent *event)
@@ -747,6 +747,7 @@ void TabBarWidget::contextMenuEvent(QContextMenuEvent *event)
 	thumbnailsAction->setCheckable(true);
 	thumbnailsAction->setChecked(SettingsManager::getOption(SettingsManager::TabBar_EnableThumbnailsOption).toBool());
 
+#ifndef __riscos__
 	connect(cycleAction, &QAction::toggled, [&](bool isEnabled)
 	{
 		SettingsManager::setOption(SettingsManager::TabBar_RequireModifierToSwitchTabOnScrollOption, !isEnabled);
@@ -755,7 +756,10 @@ void TabBarWidget::contextMenuEvent(QContextMenuEvent *event)
 	{
 		SettingsManager::setOption(SettingsManager::TabBar_EnableThumbnailsOption, areEnabled);
 	});
-
+#else
+	connect(cycleAction, SIGNAL(toggled(bool)), this, SLOT(tabBarRequireModifier(bool)));
+	connect(thumbnailsAction, SIGNAL(toggled(bool)), this, SLOT(tabBarEnableThumbnails(bool)));
+#endif
 	if (qobject_cast<ToolBarWidget*>(parentWidget()))
 	{
 		menu.addMenu(ToolBarWidget::createCustomizationMenu(ToolBarsManager::TabBar, {cycleAction, thumbnailsAction}, &menu));
@@ -781,6 +785,18 @@ void TabBarWidget::contextMenuEvent(QContextMenuEvent *event)
 	}
 }
 
+#ifdef __riscos__
+void TabBarWidget::tabBarRequireModifier(bool isEnabled)
+{
+	SettingsManager::setOption(SettingsManager::TabBar_RequireModifierToSwitchTabOnScrollOption, !isEnabled);
+}
+
+void TabBarWidget::tabBarEnableThumbnails(bool areEnabled)
+{
+	SettingsManager::setOption(SettingsManager::TabBar_EnableThumbnailsOption, areEnabled);
+}
+#endif
+
 void TabBarWidget::mousePressEvent(QMouseEvent *event)
 {
 	QTabBar::mousePressEvent(event);


