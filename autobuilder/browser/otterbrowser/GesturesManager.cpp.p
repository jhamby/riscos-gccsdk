--- src/core/GesturesManager.cpp
+++ src/core/GesturesManager.cpp
@@ -609,7 +609,7 @@ bool GesturesManager::m_afterScroll(false);
 GesturesManager::GesturesManager(QObject *parent) : QObject(parent),
 	m_reloadTimer(0)
 {
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &GesturesManager::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
 }
 
 void GesturesManager::createInstance()


