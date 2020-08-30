--- src/modules/windows/configuration/ConfigurationContentsWidget.cpp
+++ src/modules/windows/configuration/ConfigurationContentsWidget.cpp
@@ -220,16 +220,21 @@ ConfigurationContentsWidget::ConfigurationContentsWidget(const QVariantMap &para
 		m_ui->detailsWidget->hide();
 	}
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &ConfigurationContentsWidget::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int, QVariant)), this, SLOT(handleOptionChanged(int, QVariant)));
 	connect(m_ui->configurationViewWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
 	connect(m_ui->configurationViewWidget, SIGNAL(needsActionsUpdate()), this, SLOT(updateActions()));
+#ifndef __riscos__
 	connect(m_ui->configurationViewWidget, &ItemViewWidget::modified, [&]()
 	{
 		m_ui->resetAllButton->setEnabled(true);
 		m_ui->saveAllButton->setEnabled(true);
 	});
+#else
+	connect(m_ui->configurationViewWidget, SIGNAL(modified()), this, SLOT(setButtonEnabled()));
+#endif
 	connect(m_ui->configurationViewWidget->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));
 	connect(m_ui->filterLineEditWidget, SIGNAL(textChanged(QString)), m_ui->configurationViewWidget, SLOT(setFilterString(QString)));
+#ifndef __riscos__
 	connect(m_ui->resetAllButton, &QPushButton::clicked, [&]()
 	{
 		saveAll(true);
@@ -238,8 +243,30 @@ ConfigurationContentsWidget::ConfigurationContentsWidget(const QVariantMap &para
 	{
 		saveAll(false);
 	});
+#else
+	connect(m_ui->resetAllButton, SIGNAL(clicked()), this, SLOT(trueSaveAll()));
+	connect(m_ui->saveAllButton, SIGNAL(clicked()), this, SLOT(falseSaveAll()));
+#endif
 }
 
+#ifdef __riscos__
+void ConfigurationContentsWidget::setButtonEnabled()
+{
+	m_ui->resetAllButton->setEnabled(true);
+	m_ui->saveAllButton->setEnabled(true);
+}
+
+void ConfigurationContentsWidget::trueSaveAll()
+{
+	saveAll(true);
+}
+
+void ConfigurationContentsWidget::falseSaveAll()
+{
+	saveAll(false);
+}
+#endif
+
 ConfigurationContentsWidget::~ConfigurationContentsWidget()
 {
 	delete m_ui;


