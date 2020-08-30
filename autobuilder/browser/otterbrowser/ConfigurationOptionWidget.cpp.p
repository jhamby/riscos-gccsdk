--- src/modules/widgets/configurationOption/ConfigurationOptionWidget.cpp
+++ src/modules/widgets/configurationOption/ConfigurationOptionWidget.cpp
@@ -87,14 +87,14 @@ ConfigurationOptionWidget::ConfigurationOptionWidget(Window *window, const ToolB
 
 		if (toolBar && toolBar->getIdentifier() != ToolBarsManager::AddressBar)
 		{
-			connect(toolBar, &ToolBarWidget::windowChanged, this, &ConfigurationOptionWidget::setWindow);
+			connect(toolBar, SIGNAL(windowChanged(Window*)), this, SLOT(setWindow(Window*)));
 		}
 
-		connect(SettingsManager::getInstance(), &SettingsManager::hostOptionChanged, this, &ConfigurationOptionWidget::updateValue);
+		connect(SettingsManager::getInstance(), SIGNAL(hostOptionChanged(int,QVariant)), this, SLOT(updateValue(int)));
 	}
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &ConfigurationOptionWidget::handleOptionChanged);
-	connect(m_optionWidget, &OptionWidget::commitData, this, &ConfigurationOptionWidget::save);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
+	connect(m_optionWidget, SIGNAL(commitData(QWidget*)), this, SLOT(save()));
 }
 
 void ConfigurationOptionWidget::handleOptionChanged(int option, const QVariant &value)


