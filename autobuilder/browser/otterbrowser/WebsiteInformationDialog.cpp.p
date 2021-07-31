--- src/ui/WebsiteInformationDialog.cpp
+++ src/ui/WebsiteInformationDialog.cpp
@@ -261,6 +261,7 @@ WebsiteInformationDialog::WebsiteInformationDialog(WebWidget *widget, QWidget *p
 
 	setWindowTitle(tr("Information for %1").arg(host));
 
+#ifndef __riscos__
 	connect(m_ui->preferencesDetailsButton, &QPushButton::clicked, [&]()
 	{
 		Application::triggerAction(ActionsManager::WebsitePreferencesAction, {}, this);
@@ -271,8 +272,27 @@ WebsiteInformationDialog::WebsiteInformationDialog(WebWidget *widget, QWidget *p
 		dialog->setAttribute(Qt::WA_DeleteOnClose);
 		dialog->show();
 	});
+#else
+	connect(m_ui->preferencesDetailsButton, SIGNAL(clicked()), this, SLOT(setTriggerAction()));
+	connect(m_ui->certificateDetailsButton, SIGNAL(clicked()), this, SLOT(showCertificateDialog()));
+#endif
 }
 
+#ifdef __riscos__
+void WebsiteInformationDialog::setTriggerAction()
+{
+	Application::triggerAction(ActionsManager::WebsitePreferencesAction, {}, this);
+}
+
+
+void WebsiteInformationDialog::showCertificateDialog()
+{
+	CertificateDialog *dialog(new CertificateDialog(m_sslInformation.certificates));
+	dialog->setAttribute(Qt::WA_DeleteOnClose);
+	dialog->show();
+}
+#endif
+
 WebsiteInformationDialog::~WebsiteInformationDialog()
 {
 	delete m_ui;


