--- src/core/HistoryManager.cpp
+++ src/core/HistoryManager.cpp
@@ -44,7 +44,7 @@ HistoryManager::HistoryManager(QObject *parent) : QObject(parent),
 	handleOptionChanged(SettingsManager::History_RememberBrowsingOption);
 	handleOptionChanged(SettingsManager::History_StoreFaviconsOption);
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &HistoryManager::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
 }
 
 void HistoryManager::createInstance()


