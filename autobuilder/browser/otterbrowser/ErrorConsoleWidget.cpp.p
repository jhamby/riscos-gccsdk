--- src/modules/widgets/errorConsole/ErrorConsoleWidget.cpp
+++ src/modules/widgets/errorConsole/ErrorConsoleWidget.cpp
@@ -44,6 +44,7 @@ ErrorConsoleWidget::ErrorConsoleWidget(QWidget *parent) : QWidget(parent),
 
 	if (toolBar)
 	{
+#ifndef __riscos__
 		connect(m_ui->closeButton, &QToolButton::clicked, [&]()
 		{
 			const ToolBarWidget *toolBar(qobject_cast<ToolBarWidget*>(parentWidget()));
@@ -53,6 +54,9 @@ ErrorConsoleWidget::ErrorConsoleWidget(QWidget *parent) : QWidget(parent),
 				Application::getInstance()->triggerAction(ActionsManager::ShowToolBarAction, {{QLatin1String("toolBar"), toolBar->getIdentifier()}, {QLatin1String("isChecked"), false}});
 			}
 		});
+#else
+		connect(m_ui->closeButton, SIGNAL(clicked()), this, SLOT(setTriggerAction()));
+#endif
 	}
 	else
 	{
@@ -94,6 +98,18 @@ ErrorConsoleWidget::ErrorConsoleWidget(QWidget *parent) : QWidget(parent),
 	connect(m_ui->consoleView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
 }
 
+#ifdef __riscos__
+void ErrorConsoleWidget::setTriggerAction()
+{
+	const ToolBarWidget *toolBar(qobject_cast<ToolBarWidget*>(parentWidget()));
+
+	if (toolBar)
+	{
+		Application::getInstance()->triggerAction(ActionsManager::ShowToolBarAction, {{QLatin1String("toolBar"), toolBar->getIdentifier()}, {QLatin1String("isChecked"), false}});
+	}
+}
+#endif
+
 ErrorConsoleWidget::~ErrorConsoleWidget()
 {
 	delete m_ui;


