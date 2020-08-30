--- src/modules/widgets/address/AddressWidget.cpp
+++ src/modules/widgets/address/AddressWidget.cpp
@@ -59,7 +59,7 @@ AddressDelegate::AddressDelegate(const QString &highlight, ViewMode mode, QObjec
 	m_displayMode((SettingsManager::getOption(SettingsManager::AddressField_CompletionDisplayModeOption).toString() == QLatin1String("columns")) ? ColumnsMode : CompactMode),
 	m_viewMode(mode)
 {
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &AddressDelegate::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 }
 
 void AddressDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
@@ -359,11 +359,11 @@ AddressWidget::AddressWidget(Window *window, QWidget *parent) : LineEditWidget(p
 	{
 		setPlaceholderText(tr("Enter address or search…"));
 
-		connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &AddressWidget::handleOptionChanged);
+		connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 
 		if (toolBar->getIdentifier() != ToolBarsManager::AddressBar)
 		{
-			connect(toolBar, &ToolBarWidget::windowChanged, this, &AddressWidget::setWindow);
+			connect(toolBar, SIGNAL(windowChanged(Window*)), this, SLOT(setWindow(Window*)));
 		}
 	}
 


