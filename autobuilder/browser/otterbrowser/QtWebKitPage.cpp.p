--- src/modules/backends/web/qtwebkit/QtWebKitPage.cpp
+++ src/modules/backends/web/qtwebkit/QtWebKitPage.cpp
@@ -197,7 +197,7 @@ QtWebKitPage::QtWebKitPage(QtWebKitNetworkManager *networkManager, QtWebKitWebWi
 	setForwardUnsupportedContent(true);
 	handleFrameCreation(mainFrame());
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &QtWebKitPage::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
 	connect(this, SIGNAL(frameCreated(QWebFrame*)), this, SLOT(handleFrameCreation(QWebFrame*)));
 #ifndef OTTER_ENABLE_QTWEBKIT_LEGACY
 	connect(this, SIGNAL(consoleMessageReceived(MessageSource,MessageLevel,QString,int,QString)), this, SLOT(handleConsoleMessage(MessageSource,MessageLevel,QString,int,QString)));


