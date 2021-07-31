--- src/ui/WebWidget.cpp
+++ src/ui/WebWidget.cpp
@@ -288,6 +288,7 @@ void WebWidget::handleWindowCloseRequest()
 	dialog->setCheckBox(tr("Do not show this message again"), false);
 
 	connect(this, SIGNAL(aboutToReload()), dialog, SLOT(close()));
+#ifndef __riscos__
 	connect(dialog, &ContentsDialog::finished, [&](int result, bool isChecked)
 	{
 		const bool isAccepted(result == QDialog::Accepted);
@@ -302,10 +303,30 @@ void WebWidget::handleWindowCloseRequest()
 			emit requestedCloseWindow();
 		}
 	});
+#else
+	connect(dialog, SIGNAL(finished(int, bool)), this, SLOT(setFinished(int, bool)));
+#endif
 
 	showDialog(dialog, false);
 }
 
+#ifdef __riscos__
+void WebWidget::setFinished(int result, bool isChecked)
+{
+	const bool isAccepted(result == QDialog::Accepted);
+
+	if (isChecked)
+	{
+		SettingsManager::setOption(SettingsManager::Permissions_ScriptsCanCloseWindowsOption, (isAccepted ? QLatin1String("allow") : QLatin1String("disallow")));
+	}
+
+	if (isAccepted)
+	{
+		emit requestedCloseWindow();
+	}
+}
+#endif
+
 void WebWidget::notifyFillPasswordActionStateChanged()
 {
 	emit actionsStateChanged(QVector<int>(ActionsManager::FillPasswordAction));


