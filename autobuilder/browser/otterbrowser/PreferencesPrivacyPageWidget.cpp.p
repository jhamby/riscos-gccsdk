--- src/ui/preferences/PreferencesPrivacyPageWidget.cpp
+++ src/ui/preferences/PreferencesPrivacyPageWidget.cpp
@@ -81,11 +81,23 @@ PreferencesPrivacyPageWidget::PreferencesPrivacyPageWidget(QWidget *parent) : QW
 	connect(m_ui->thirdPartyCookiesExceptionsButton, SIGNAL(clicked(bool)), this, SLOT(setupThirdPartyCookiesExceptions()));
 	connect(m_ui->clearHistoryCheckBox, SIGNAL(toggled(bool)), m_ui->clearHistoryButton, SLOT(setEnabled(bool)));
 	connect(m_ui->clearHistoryButton, SIGNAL(clicked()), this, SLOT(setupClearHistory()));
+#ifndef __riscos__
 	connect(m_ui->managePasswordsButton, &QPushButton::clicked, [&]()
 	{
 		Application::triggerAction(ActionsManager::PasswordsAction, {}, this);
 	});
+#else
+	connect(m_ui->managePasswordsButton, SIGNAL(clicked()), this, SLOT(viewPasswords()));
+#endif
+
+}
+
+#ifdef __riscos__
+void PreferencesPrivacyPageWidget::viewPasswords()
+{
+	Application::triggerAction(ActionsManager::PasswordsAction, {}, this);
 }
+#endif
 
 PreferencesPrivacyPageWidget::~PreferencesPrivacyPageWidget()
 {


