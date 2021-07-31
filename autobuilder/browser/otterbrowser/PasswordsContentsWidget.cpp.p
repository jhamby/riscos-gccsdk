--- src/modules/windows/passwords/PasswordsContentsWidget.cpp
+++ src/modules/windows/passwords/PasswordsContentsWidget.cpp
@@ -115,6 +115,8 @@ void PasswordsContentsWidget::populatePasswords()
 		emit loadingStateChanged(WebWidget::FinishedLoadingState);
 
 		connect(PasswordsManager::getInstance(), SIGNAL(passwordsModified()), this, SLOT(populatePasswords()));
+
+#ifndef __riscos__
 		connect(m_ui->passwordsViewWidget->selectionModel(), &QItemSelectionModel::selectionChanged, [&](const QItemSelection &selected, const QItemSelection &deselected)
 		{
 			Q_UNUSED(selected)
@@ -122,9 +124,20 @@ void PasswordsContentsWidget::populatePasswords()
 
 			emit actionsStateChanged(QVector<int>({ActionsManager::DeleteAction}));
 		});
+#else
+
+		connect(m_ui->passwordsViewWidget->selectionModel(), SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)), this, SLOT(setActionStateChanged()));
+#endif
 	}
 }
 
+#ifdef __riscos__
+void PasswordsContentsWidget::setActionStateChanged()
+{
+	emit actionsStateChanged(QVector<int>({ActionsManager::DeleteAction}));
+}
+#endif
+
 void PasswordsContentsWidget::removePasswords()
 {
 	const QModelIndexList indexes(m_ui->passwordsViewWidget->selectionModel()->selectedIndexes());


