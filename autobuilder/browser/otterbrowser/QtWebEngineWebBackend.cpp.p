--- src/modules/backends/web/qtwebengine/QtWebEngineWebBackend.cpp
+++ src/modules/backends/web/qtwebengine/QtWebEngineWebBackend.cpp
@@ -247,8 +247,8 @@ WebWidget* QtWebEngineWebBackend::createWidget(const QVariantMap &parameters, Co
 		handleOptionChanged(SettingsManager::Content_DefaultFontSizeOption);
 		handleOptionChanged(SettingsManager::Permissions_EnableFullScreenOption);
 
-		connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &QtWebEngineWebBackend::handleOptionChanged);
-		connect(QWebEngineProfile::defaultProfile(), &QWebEngineProfile::downloadRequested, this, &QtWebEngineWebBackend::downloadFile);
+		connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
+		connect(QWebEngineProfile::defaultProfile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)), this, SLOT(downloadFile(QWebEngineDownloadItem*)));
 	}
 
 	return new QtWebEngineWebWidget(parameters, this, parent);


