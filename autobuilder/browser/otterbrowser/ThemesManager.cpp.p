--- src/core/ThemesManager.cpp
+++ src/core/ThemesManager.cpp
@@ -49,7 +49,7 @@ ThemesManager::ThemesManager(QObject *parent) : QObject(parent)
 
 	handleOptionChanged(SettingsManager::Interface_IconThemePathOption, SettingsManager::getOption(SettingsManager::Interface_IconThemePathOption));
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &ThemesManager::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 }
 
 void ThemesManager::createInstance()


