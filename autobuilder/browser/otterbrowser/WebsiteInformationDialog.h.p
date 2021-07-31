--- src/ui/WebsiteInformationDialog.h
+++ src/ui/WebsiteInformationDialog.h
@@ -42,6 +42,10 @@ public:
 protected:
 	void changeEvent(QEvent *event) override;
 
+protected slots:
+	void setTriggerAction();
+	void showCertificateDialog();
+
 private:
 	WebWidget::SslInformation m_sslInformation;
 	Ui::WebsiteInformationDialog *m_ui;


