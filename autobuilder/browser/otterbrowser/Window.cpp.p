--- src/ui/Window.cpp
+++ src/ui/Window.cpp
@@ -968,6 +968,7 @@ bool Window::event(QEvent *event)
 
 		if (subWindow)
 		{
+#ifdef __riscos__
 			connect(subWindow, &QMdiSubWindow::windowStateChanged, [&](Qt::WindowStates oldState, Qt::WindowStates newState)
 			{
 				Q_UNUSED(oldState)
@@ -975,10 +976,20 @@ bool Window::event(QEvent *event)
 
 				emit actionsStateChanged(QVector<int>({ActionsManager::MaximizeTabAction, ActionsManager::MinimizeTabAction, ActionsManager::RestoreTabAction}));
 			});
+#else
+			connect(subWindow, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)), this, SLOT(setWindowStateChanged()));
+#endif
 		}
 	}
 
 	return QWidget::event(event);
 }
 
+#ifdef __riscos__
+void Window::setWindowStateChanged()
+{
+	emit actionsStateChanged(QVector<int>({ActionsManager::MaximizeTabAction, ActionsManager::MinimizeTabAction, ActionsManager::RestoreTabAction}));
+}
+#endif
+
 }


