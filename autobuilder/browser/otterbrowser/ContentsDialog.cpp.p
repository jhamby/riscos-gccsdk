--- src/ui/ContentsDialog.cpp
+++ src/ui/ContentsDialog.cpp
@@ -152,12 +152,17 @@ ContentsDialog::ContentsDialog(const QIcon &icon, const QString &title, const QS
 			{
 				connect(this, SIGNAL(accepted(bool)), dialog, SLOT(accept()));
 				connect(this, SIGNAL(rejected(bool)), dialog, SLOT(reject()));
+#ifndef __riscos__
 				connect(dialog, &QDialog::finished, [&](int result)
 				{
 					m_isAccepted = (result == QDialog::Accepted);
 
 					close();
 				});
+#else
+				connect(dialog, SIGNAL(finished(int)), this, SLOT(setFinished(int)));
+
+#endif
 			}
 		}
 
@@ -184,6 +189,15 @@ ContentsDialog::ContentsDialog(const QIcon &icon, const QString &title, const QS
 	installEventFilter(this);
 }
 
+#ifdef __riscos__
+void ContentsDialog::setFinished(int result)
+{
+	m_isAccepted = (result == QDialog::Accepted);
+
+	close();
+}
+#endif
+
 void ContentsDialog::showEvent(QShowEvent *event)
 {
 	if (m_scrollArea)


