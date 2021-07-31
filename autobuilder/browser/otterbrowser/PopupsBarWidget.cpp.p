--- src/modules/windows/web/PopupsBarWidget.cpp
+++ src/modules/windows/web/PopupsBarWidget.cpp
@@ -74,7 +74,7 @@ PopupsBarWidget::PopupsBarWidget(const QUrl &parentUrl, bool isPrivate, QWidget
 
 	handleOptionChanged(SettingsManager::Permissions_ScriptsCanOpenWindowsOption);
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &PopupsBarWidget::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
 	connect(m_popupsGroup, SIGNAL(triggered(QAction*)), this, SLOT(setPolicy(QAction*)));
 	connect(m_popupsMenu, SIGNAL(triggered(QAction*)), this, SLOT(openUrl(QAction*)));
 	connect(m_ui->closeButton, SIGNAL(clicked()), this, SIGNAL(requestedClose()));


