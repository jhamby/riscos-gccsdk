--- src/modules/widgets/search/SearchWidget.cpp
+++ src/modules/widgets/search/SearchWidget.cpp
@@ -157,11 +157,11 @@ SearchWidget::SearchWidget(Window *window, QWidget *parent) : LineEditWidget(par
 		connect(toolBar, SIGNAL(windowChanged(Window*)), this, SLOT(setWindow(Window*)));
 	}
 
-	connect(SearchEnginesManager::getInstance(), &SearchEnginesManager::searchEnginesModified, this, &SearchWidget::storeCurrentSearchEngine);
-	connect(SearchEnginesManager::getInstance(), &SearchEnginesManager::searchEnginesModelModified, this, &SearchWidget::restoreCurrentSearchEngine);
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &SearchWidget::handleOptionChanged);
-	connect(this, &SearchWidget::textChanged, this, &SearchWidget::setQuery);
-	connect(this, &SearchWidget::textDropped, this, &SearchWidget::sendRequest);
+	connect(SearchEnginesManager::getInstance(), SIGNAL(searchEnginesModified()), this, SLOT(storeCurrentSearchEngine()));
+	connect(SearchEnginesManager::getInstance(), SIGNAL(searchEnginesModelModified()), this, SLOT(restoreCurrentSearchEngine()));
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
+	connect(this, SIGNAL(textChanged(QString)), this, SLOT(setQuery(QString)));
+	connect(this, SIGNAL(textDropped(QString)), this, SLOT(sendRequest(QString)));
 
 	setWindow(window);
 }


