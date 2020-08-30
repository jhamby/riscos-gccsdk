--- src/ui/WorkspaceWidget.cpp
+++ src/ui/WorkspaceWidget.cpp
@@ -222,7 +222,7 @@ WorkspaceWidget::WorkspaceWidget(MainWindow *parent) : QWidget(parent),
 
 	if (!m_mdi)
 	{
-		connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &WorkspaceWidget::handleOptionChanged);
+		connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 	}
 }
 
@@ -278,7 +278,7 @@ void WorkspaceWidget::createMdi()
 		return;
 	}
 
-	disconnect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &WorkspaceWidget::handleOptionChanged);
+	disconnect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
 
 	Window *activeWindow(m_activeWindow);
 
@@ -308,7 +308,7 @@ void WorkspaceWidget::createMdi()
 		markAsRestored();
 	}
 
-	connect(m_mdi, &MdiWidget::customContextMenuRequested, this, &WorkspaceWidget::showContextMenu);
+	connect(m_mdi, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
 }
 
 void WorkspaceWidget::triggerAction(int identifier, const QVariantMap &parameters)


