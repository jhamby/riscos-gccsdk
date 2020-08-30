--- src/modules/windows/web/WebContentsWidget.cpp
+++ src/modules/windows/web/WebContentsWidget.cpp
@@ -1198,31 +1198,31 @@ void WebContentsWidget::setWidget(WebWidget *widget, const QVariantMap &paramete
 
 	handleLoadingStateChange(m_webWidget->getLoadingState());
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &WebContentsWidget::handleOptionChanged);
-	connect(m_webWidget, &WebWidget::aboutToNavigate, this, &WebContentsWidget::aboutToNavigate);
-	connect(m_webWidget, &WebWidget::aboutToNavigate, this, &WebContentsWidget::closePopupsBar);
-	connect(m_webWidget, &WebWidget::needsAttention, this, &WebContentsWidget::needsAttention);
-	connect(m_webWidget, &WebWidget::requestedNewWindow, this, &WebContentsWidget::notifyRequestedNewWindow);
-	connect(m_webWidget, &WebWidget::requestedSearch, this, &WebContentsWidget::requestedSearch);
-	connect(m_webWidget, &WebWidget::requestedPopupWindow, this, &WebContentsWidget::handlePopupWindowRequest);
-	connect(m_webWidget, &WebWidget::requestedPermission, this, &WebContentsWidget::handlePermissionRequest);
-	connect(m_webWidget, &WebWidget::requestedSavePassword, this, &WebContentsWidget::handleSavePasswordRequest);
-	connect(m_webWidget, &WebWidget::requestedGeometryChange, this, &WebContentsWidget::requestedGeometryChange);
-	connect(m_webWidget, &WebWidget::requestedInspectorVisibilityChange, this, &WebContentsWidget::handleInspectorVisibilityChangeRequest);
-	connect(m_webWidget, &WebWidget::statusMessageChanged, this, &WebContentsWidget::statusMessageChanged);
-	connect(m_webWidget, &WebWidget::titleChanged, this, &WebContentsWidget::titleChanged);
-	connect(m_webWidget, &WebWidget::urlChanged, this, &WebContentsWidget::urlChanged);
-	connect(m_webWidget, &WebWidget::urlChanged, this, &WebContentsWidget::handleUrlChange);
-	connect(m_webWidget, &WebWidget::iconChanged, this, &WebContentsWidget::iconChanged);
-	connect(m_webWidget, &WebWidget::requestBlocked, this, &WebContentsWidget::requestBlocked);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
+	connect(m_webWidget, SIGNAL(aboutToNavigate()), this, SIGNAL(aboutToNavigate()));
+	connect(m_webWidget, SIGNAL(aboutToNavigate()), this, SLOT(closePopupsBar()));
+	connect(m_webWidget, SIGNAL(needsAttention()), this, SIGNAL(needsAttention()));
+	connect(m_webWidget, SIGNAL(requestedNewWindow(WebWidget*,SessionsManager::OpenHints)), this, SLOT(notifyRequestedNewWindow(WebWidget*,SessionsManager::OpenHints)));
+	connect(m_webWidget, SIGNAL(requestedSearch(QString,QString,SessionsManager::OpenHints)), this, SIGNAL(requestedSearch(QString,QString,SessionsManager::OpenHints)));
+	connect(m_webWidget, SIGNAL(requestedPopupWindow(QUrl,QUrl)), this, SLOT(handlePopupWindowRequest(QUrl,QUrl)));
+	connect(m_webWidget, SIGNAL(requestedPermission(WebWidget::FeaturePermission,QUrl,bool)), this, SLOT(handlePermissionRequest(WebWidget::FeaturePermission,QUrl,bool)));
+	connect(m_webWidget, SIGNAL(requestedSavePassword(PasswordsManager::PasswordInformation,bool)), this, SLOT(handleSavePasswordRequest(PasswordsManager::PasswordInformation,bool)));
+	connect(m_webWidget, SIGNAL(requestedGeometryChange(QRect)), this, SIGNAL(requestedGeometryChange(QRect)));
+	connect(m_webWidget, SIGNAL(requestedInspectorVisibilityChange(bool)), this, SLOT(handleInspectorVisibilityChangeRequest(bool)));
+	connect(m_webWidget, SIGNAL(statusMessageChanged(QString)), this, SIGNAL(statusMessageChanged(QString)));
+	connect(m_webWidget, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));
+	connect(m_webWidget, SIGNAL(urlChanged(QUrl)), this, SIGNAL(urlChanged(QUrl)));
+	connect(m_webWidget, SIGNAL(urlChanged(QUrl)), this, SLOT(handleUrlChange(QUrl)));
+	connect(m_webWidget, SIGNAL(iconChanged(QIcon)), this, SIGNAL(iconChanged(QIcon)));
+	connect(m_webWidget, SIGNAL(requestBlocked(NetworkManager::ResourceInformation)), this, SIGNAL(requestBlocked(NetworkManager::ResourceInformation)));
 	connect(m_webWidget, SIGNAL(actionsStateChanged(QVector<int>)), this, SIGNAL(actionsStateChanged(QVector<int>)));
 	connect(m_webWidget, SIGNAL(actionsStateChanged(ActionsManager::ActionDefinition::ActionCategories)), this, SIGNAL(actionsStateChanged(ActionsManager::ActionDefinition::ActionCategories)));
-	connect(m_webWidget, &WebWidget::contentStateChanged, this, &WebContentsWidget::contentStateChanged);
-	connect(m_webWidget, &WebWidget::loadingStateChanged, this, &WebContentsWidget::loadingStateChanged);
-	connect(m_webWidget, &WebWidget::loadingStateChanged, this, &WebContentsWidget::handleLoadingStateChange);
-	connect(m_webWidget, &WebWidget::pageInformationChanged, this, &WebContentsWidget::pageInformationChanged);
-	connect(m_webWidget, &WebWidget::optionChanged, this, &WebContentsWidget::optionChanged);
-	connect(m_webWidget, &WebWidget::zoomChanged, this, &WebContentsWidget::zoomChanged);
+	connect(m_webWidget, SIGNAL(contentStateChanged(WebWidget::ContentStates)), this, SIGNAL(contentStateChanged(WebWidget::ContentStates)));
+	connect(m_webWidget, SIGNAL(loadingStateChanged(WebWidget::LoadingState)), this, SIGNAL(loadingStateChanged(WebWidget::LoadingState)));
+	connect(m_webWidget, SIGNAL(loadingStateChanged(WebWidget::LoadingState)), this, SLOT(handleLoadingStateChange(WebWidget::LoadingState)));
+	connect(m_webWidget, SIGNAL(pageInformationChanged(WebWidget::PageInformation,QVariant)), this, SIGNAL(pageInformationChanged(WebWidget::PageInformation,QVariant)));
+	connect(m_webWidget, SIGNAL(optionChanged(int,QVariant)), this, SIGNAL(optionChanged(int,QVariant)));
+	connect(m_webWidget, SIGNAL(zoomChanged(int)), this, SIGNAL(zoomChanged(int)));
 
 	emit webWidgetChanged();
 }


