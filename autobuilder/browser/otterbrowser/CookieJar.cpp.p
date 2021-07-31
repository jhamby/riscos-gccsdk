--- src/core/CookieJar.cpp
+++ src/core/CookieJar.cpp
@@ -77,7 +77,7 @@ CookieJar::CookieJar(bool isPrivate, QObject *parent) : QNetworkCookieJar(parent
 	handleOptionChanged(SettingsManager::Network_CookiesPolicyOption, SettingsManager::getOption(SettingsManager::Network_CookiesPolicyOption));
 	setAllCookies(allCookies);
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &CookieJar::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 }
 
 void CookieJar::timerEvent(QTimerEvent *event)


