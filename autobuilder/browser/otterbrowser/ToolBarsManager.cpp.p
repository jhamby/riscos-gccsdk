--- src/core/ToolBarsManager.cpp
+++ src/core/ToolBarsManager.cpp
@@ -53,7 +53,7 @@ ToolBarsManager::ToolBarsManager(QObject *parent) : QObject(parent),
 	Q_UNUSED(QT_TRANSLATE_NOOP("actions", "Status Bar"))
 	Q_UNUSED(QT_TRANSLATE_NOOP("actions", "Error Console"))
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &ToolBarsManager::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 }
 
 void ToolBarsManager::createInstance()


