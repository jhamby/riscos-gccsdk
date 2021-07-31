--- src/modules/backends/web/qtwebkit/QtWebKitWebBackend.cpp
+++ src/modules/backends/web/qtwebkit/QtWebKitWebBackend.cpp
@@ -173,7 +173,7 @@ WebWidget* QtWebKitWebBackend::createWidget(const QVariantMap &parameters, Conte
 		handleOptionChanged(SettingsManager::Content_DefaultFontSizeOption);
 		handleOptionChanged(SettingsManager::Permissions_EnableFullScreenOption);
 
-		connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &QtWebKitWebBackend::handleOptionChanged);
+		connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
 	}
 
 	QtWebKitWebWidget *widget(new QtWebKitWebWidget(parameters, this, nullptr, parent));


