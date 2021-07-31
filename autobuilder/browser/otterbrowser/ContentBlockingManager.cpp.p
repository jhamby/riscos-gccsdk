--- src/core/ContentBlockingManager.cpp
+++ src/core/ContentBlockingManager.cpp
@@ -46,7 +46,7 @@ ContentBlockingManager::ContentBlockingManager(QObject *parent) : QObject(parent
 
 	handleOptionChanged(SettingsManager::ContentBlocking_CosmeticFiltersModeOption, SettingsManager::getOption(SettingsManager::ContentBlocking_CosmeticFiltersModeOption).toString());
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &ContentBlockingManager::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 }
 
 void ContentBlockingManager::createInstance()


