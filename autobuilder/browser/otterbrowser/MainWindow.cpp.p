--- src/ui/MainWindow.cpp
+++ src/ui/MainWindow.cpp
@@ -135,14 +135,14 @@ MainWindow::MainWindow(const QVariantMap &parameters, const SessionMainWindow &s
 
 	setCentralWidget(m_workspace);
 
-	connect(ActionsManager::getInstance(), &ActionsManager::shortcutsChanged, this, &MainWindow::updateShortcuts);
-	connect(SessionsManager::getInstance(), &SessionsManager::requestedRemoveStoredUrl, this, &MainWindow::removeStoredUrl);
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &MainWindow::handleOptionChanged);
-	connect(ToolBarsManager::getInstance(), &ToolBarsManager::toolBarAdded, this, &MainWindow::handleToolBarAdded);
-	connect(ToolBarsManager::getInstance(), &ToolBarsManager::toolBarModified, this, &MainWindow::handleToolBarModified);
-	connect(ToolBarsManager::getInstance(), &ToolBarsManager::toolBarMoved, this, &MainWindow::handleToolBarMoved);
-	connect(ToolBarsManager::getInstance(), &ToolBarsManager::toolBarRemoved, this, &MainWindow::handleToolBarRemoved);
-	connect(TransfersManager::getInstance(), &TransfersManager::transferStarted, this, &MainWindow::handleTransferStarted);
+	connect(ActionsManager::getInstance(), SIGNAL(shortcutsChanged()), this, SLOT(updateShortcuts()));
+	connect(SessionsManager::getInstance(), SIGNAL(requestedRemoveStoredUrl(QString)), this, SLOT(removeStoredUrl(QString)));
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
+	connect(ToolBarsManager::getInstance(), SIGNAL(toolBarAdded(int)), this, SLOT(handleToolBarAdded(int)));
+	connect(ToolBarsManager::getInstance(), SIGNAL(toolBarModified(int)), this, SLOT(handleToolBarModified(int)));
+	connect(ToolBarsManager::getInstance(), SIGNAL(toolBarMoved(int)), this, SLOT(handleToolBarMoved(int)));
+	connect(ToolBarsManager::getInstance(), SIGNAL(toolBarRemoved(int)), this, SLOT(handleToolBarRemoved(int)));
+	connect(TransfersManager::getInstance(), SIGNAL(transferStarted(Transfer*)), this, SLOT(handleTransferStarted()));
 
 	if (session.geometry.isEmpty())
 	{
@@ -1200,6 +1200,15 @@ void MainWindow::clearClosedWindows()
 	}
 }
 
+#ifdef __riscos__
+
+void MainWindow::setAlert()
+{
+
+	QApplication::alert(this);
+}
+
+#endif
 void MainWindow::addWindow(Window *window, SessionsManager::OpenHints hints, int index, const WindowState &state, bool isAlwaysOnTop)
 {
 	if (!window)
@@ -1271,10 +1280,15 @@ void MainWindow::addWindow(Window *window, SessionsManager::OpenHints hints, int
 		}
 	}
 
+#ifndef __riscos__
 	connect(window, &Window::needsAttention, [&]()
 	{
 		QApplication::alert(this);
 	});
+#else
+
+	connect(window, SIGNAL(needsAttention()), this, SLOT(setAlert()));
+#endif
 	connect(window, SIGNAL(titleChanged(QString)), this, SLOT(updateWindowTitle()));
 	connect(window, SIGNAL(requestedSearch(QString,QString,SessionsManager::OpenHints)), this, SLOT(search(QString,QString,SessionsManager::OpenHints)));
 	connect(window, SIGNAL(requestedNewWindow(ContentsWidget*,SessionsManager::OpenHints)), this, SLOT(openWindow(ContentsWidget*,SessionsManager::OpenHints)));
@@ -2501,7 +2515,7 @@ Shortcut::Shortcut(int identifier, const QKeySequence &sequence, const QVariantM
 	m_parameters(parameters),
 	m_identifier(identifier)
 {
-	connect(this, &Shortcut::activated, this, &Shortcut::triggerAction);
+	connect(this, SIGNAL(activated()), this, SLOT(triggerAction()));
 }
 
 void Shortcut::triggerAction()


