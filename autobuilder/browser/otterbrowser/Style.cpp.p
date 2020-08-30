--- src/ui/Style.cpp
+++ src/ui/Style.cpp
@@ -33,7 +33,7 @@ namespace Otter
 Style::Style(const QString &name) : QProxyStyle(name.isEmpty() ? nullptr : QStyleFactory::create(name)),
 	m_areToolTipsEnabled(SettingsManager::getOption(SettingsManager::Browser_ToolTipsModeOption).toString() != QLatin1String("disabled"))
 {
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &Style::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 }
 
 void Style::drawDropZone(const QLine &line, QPainter *painter) const


