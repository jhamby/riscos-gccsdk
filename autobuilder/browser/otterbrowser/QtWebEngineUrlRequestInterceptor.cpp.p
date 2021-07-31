--- src/modules/backends/web/qtwebengine/QtWebEngineUrlRequestInterceptor.cpp
+++ src/modules/backends/web/qtwebengine/QtWebEngineUrlRequestInterceptor.cpp
@@ -36,8 +36,8 @@ QtWebEngineUrlRequestInterceptor::QtWebEngineUrlRequestInterceptor(QObject *pare
 {
 	QTimer::singleShot(1800000, this, SLOT(clearContentBlockingInformation()));
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &QtWebEngineUrlRequestInterceptor::handleOptionChanged);
-	connect(SettingsManager::getInstance(), &SettingsManager::hostOptionChanged, this, &QtWebEngineUrlRequestInterceptor::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant,QUrl)), this, SLOT(handleOptionChanged(int)));
 }
 
 void QtWebEngineUrlRequestInterceptor::clearContentBlockingInformation()


