--- src/modules/windows/web/StartPageWidget.cpp
+++ src/modules/windows/web/StartPageWidget.cpp
@@ -321,9 +321,9 @@ StartPageWidget::StartPageWidget(Window *parent) : QScrollArea(parent),
 	handleOptionChanged(SettingsManager::StartPage_BackgroundPathOption, SettingsManager::getOption(SettingsManager::StartPage_BackgroundPathOption));
 	handleOptionChanged(SettingsManager::StartPage_ShowSearchFieldOption, SettingsManager::getOption(SettingsManager::StartPage_ShowSearchFieldOption));
 
-	connect(m_model, &StartPageModel::modelModified, this, &StartPageWidget::updateSize);
-	connect(m_model, &StartPageModel::isReloadingTileChanged, this, &StartPageWidget::handleIsReloadingTileChanged);
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &StartPageWidget::handleOptionChanged);
+	connect(m_model, SIGNAL(modelModified()), this, SLOT(updateSize()));
+	connect(m_model, SIGNAL(isReloadingTileChanged(QModelIndex)), this, SLOT(handleIsReloadingTileChanged(QModelIndex)));
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 }
 
 StartPageWidget::~StartPageWidget()
@@ -465,7 +465,7 @@ void StartPageWidget::addTile()
 	OpenAddressDialog dialog(ActionExecutor::Object(), this);
 	dialog.setWindowTitle(tr("Add Tile"));
 
-	connect(&dialog, &OpenAddressDialog::requestedOpenUrl, m_model, &StartPageModel::addTile);
+	connect(&dialog, SIGNAL(requestedLoadUrl(QUrl,SessionsManager::OpenHints)), m_model, SLOT(addTile(QUrl)));
 
 	m_ignoreEnter = true;
 


