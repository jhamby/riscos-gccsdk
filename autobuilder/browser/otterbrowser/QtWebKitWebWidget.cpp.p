--- src/modules/backends/web/qtwebkit/QtWebKitWebWidget.cpp
+++ src/modules/backends/web/qtwebkit/QtWebKitWebWidget.cpp
@@ -130,7 +130,7 @@ QtWebKitWebWidget::QtWebKitWebWidget(const QVariantMap &parameters, WebBackend *
 	handleOptionChanged(SettingsManager::History_BrowsingLimitAmountWindowOption, SettingsManager::getOption(SettingsManager::History_BrowsingLimitAmountWindowOption));
 	setZoom(SettingsManager::getOption(SettingsManager::Content_DefaultZoomOption).toInt());
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &QtWebKitWebWidget::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 	connect(m_page, SIGNAL(requestedNewWindow(WebWidget*,SessionsManager::OpenHints)), this, SIGNAL(requestedNewWindow(WebWidget*,SessionsManager::OpenHints)));
 	connect(m_page, SIGNAL(requestedPopupWindow(QUrl,QUrl)), this, SIGNAL(requestedPopupWindow(QUrl,QUrl)));
 	connect(m_page, SIGNAL(saveFrameStateRequested(QWebFrame*,QWebHistoryItem*)), this, SLOT(saveState(QWebFrame*,QWebHistoryItem*)));
@@ -138,10 +138,14 @@ QtWebKitWebWidget::QtWebKitWebWidget(const QVariantMap &parameters, WebBackend *
 	connect(m_page, SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(downloadFile(QNetworkRequest)));
 	connect(m_page, SIGNAL(unsupportedContent(QNetworkReply*)), this, SLOT(downloadFile(QNetworkReply*)));
 	connect(m_page, SIGNAL(linkHovered(QString,QString,QString)), this, SLOT(handleLinkHovered(QString)));
+#ifndef __riscos__
 	connect(m_page, &QWebPage::microFocusChanged, [&]()
 	{
 		emit actionsStateChanged(ActionsManager::ActionDefinition::EditingCategory);
 	});
+#else
+	connect(m_page, SIGNAL(microFocusChanged()), this, SLOT(setActionsStateChanged()));
+#endif
 	connect(m_page, SIGNAL(printRequested(QWebFrame*)), this, SLOT(handlePrintRequest(QWebFrame*)));
 	connect(m_page, SIGNAL(windowCloseRequested()), this, SLOT(handleWindowCloseRequest()));
 #ifndef OTTER_ENABLE_QTWEBKIT_LEGACY
@@ -167,11 +171,28 @@ QtWebKitWebWidget::QtWebKitWebWidget(const QVariantMap &parameters, WebBackend *
 	connect(m_networkManager, SIGNAL(pageInformationChanged(WebWidget::PageInformation,QVariant)), this, SIGNAL(pageInformationChanged(WebWidget::PageInformation,QVariant)));
 	connect(m_networkManager, SIGNAL(requestBlocked(NetworkManager::ResourceInformation)), this, SIGNAL(requestBlocked(NetworkManager::ResourceInformation)));
 	connect(m_networkManager, SIGNAL(contentStateChanged(WebWidget::ContentStates)), this, SLOT(notifyContentStateChanged()));
+#ifndef __riscos__
 	connect(new QShortcut(QKeySequence(QKeySequence::SelectAll), this, nullptr, nullptr, Qt::WidgetWithChildrenShortcut), &QShortcut::activated, [&]()
 	{
 		triggerAction(ActionsManager::SelectAllAction);
 	});
+#else
+	connect(new QShortcut(QKeySequence(QKeySequence::SelectAll), this, nullptr, nullptr, Qt::WidgetWithChildrenShortcut), SIGNAL(activated()), this, SLOT(setTriggerAction()));
+#endif
+}
+
+#ifdef __riscos__
+void QtWebKitWebWidget::setActionsStateChanged()
+{
+	emit actionsStateChanged(ActionsManager::ActionDefinition::EditingCategory);
+}
+
+void QtWebKitWebWidget::setTriggerAction()
+{
+	triggerAction(ActionsManager::SelectAllAction);
 }
+#endif
+
 
 QtWebKitWebWidget::~QtWebKitWebWidget()
 {


