--- src/browsermainwindow.cpp.orig	2015-04-10 20:47:29.368351907 +0100
+++ src/browsermainwindow.cpp	2015-04-11 14:37:03.598005676 +0100
@@ -224,9 +224,7 @@
 #if defined(Q_WS_MAC)
     setWindowIcon(QIcon());
 #endif
-#if defined(Q_WS_X11)
     setWindowRole(QLatin1String("browser"));
-#endif
     retranslate();
 }
 
@@ -538,7 +536,6 @@
     m_fileQuit->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
     m_fileMenu->addAction(m_fileQuit);
 
-#if QT_VERSION >= 0x040600 && defined(Q_WS_X11)
     m_fileNewWindowAction->setIcon(QIcon::fromTheme(QLatin1String("window-new")));
     m_fileOpenFileAction->setIcon(QIcon::fromTheme(QLatin1String("document-open")));
     m_filePrintPreviewAction->setIcon(QIcon::fromTheme(QLatin1String("document-print-preview")));
@@ -546,7 +543,6 @@
     m_fileSaveAsAction->setIcon(QIcon::fromTheme(QLatin1String("document-save-as")));
     m_fileCloseWindow->setIcon(QIcon::fromTheme(QLatin1String("window-close")));
     m_fileQuit->setIcon(QIcon::fromTheme(QLatin1String("application-exit")));
-#endif
 
     // Edit
     m_editMenu = new QMenu(menuBar());
@@ -594,7 +590,6 @@
     connect(m_editFindPreviousAction, SIGNAL(triggered()), this, SLOT(editFindPrevious()));
     m_editMenu->addAction(m_editFindPreviousAction);
 
-#if QT_VERSION >= 0x040600 && defined(Q_WS_X11)
     m_editUndoAction->setIcon(QIcon::fromTheme(QLatin1String("edit-undo")));
     m_editRedoAction->setIcon(QIcon::fromTheme(QLatin1String("edit-redo")));
     m_editCutAction->setIcon(QIcon::fromTheme(QLatin1String("edit-cut")));
@@ -602,7 +597,6 @@
     m_editPasteAction->setIcon(QIcon::fromTheme(QLatin1String("edit-paste")));
     m_editSelectAllAction->setIcon(QIcon::fromTheme(QLatin1String("edit-select-all")));
     m_editFindAction->setIcon(QIcon::fromTheme(QLatin1String("edit-find")));
-#endif
 
     // View
     m_viewMenu = new QMenu(menuBar());
@@ -712,14 +706,12 @@
 
     m_stopIcon = style()->standardIcon(QStyle::SP_BrowserStop);
     m_reloadIcon = style()->standardIcon(QStyle::SP_BrowserReload);
-#if QT_VERSION >= 0x040600 && defined(Q_WS_X11)
     m_viewStopAction->setIcon(m_stopIcon);
     m_viewReloadAction->setIcon(m_reloadIcon);
     m_viewZoomInAction->setIcon(QIcon::fromTheme(QLatin1String("zoom-in")));
     m_viewZoomNormalAction->setIcon(QIcon::fromTheme(QLatin1String("zoom-original")));
     m_viewZoomOutAction->setIcon(QIcon::fromTheme(QLatin1String("zoom-out")));
     m_viewFullScreenAction->setIcon(QIcon::fromTheme(QLatin1String("view-fullscreen")));
-#endif
 
     // History
     m_historyMenu = new HistoryMenu(this);
@@ -731,16 +723,10 @@
     m_historyBackAction = new QAction(this);
     m_tabWidget->addWebAction(m_historyBackAction, QWebPage::Back);
     m_historyBackAction->setShortcuts(QKeySequence::Back);
-#if QT_VERSION < 0x040600 || (QT_VERSION >= 0x040600 && !defined(Q_WS_X11))
-    m_historyBackAction->setIconVisibleInMenu(false);
-#endif
 
     m_historyForwardAction = new QAction(this);
     m_tabWidget->addWebAction(m_historyForwardAction, QWebPage::Forward);
     m_historyForwardAction->setShortcuts(QKeySequence::Forward);
-#if QT_VERSION < 0x040600 || (QT_VERSION >= 0x040600 && !defined(Q_WS_X11))
-    m_historyForwardAction->setIconVisibleInMenu(false);
-#endif
 
     m_historyHomeAction = new QAction(this);
     connect(m_historyHomeAction, SIGNAL(triggered()), this, SLOT(goHome()));
@@ -757,10 +743,8 @@
     historyActions.append(m_tabWidget->recentlyClosedTabsAction());
     historyActions.append(m_historyRestoreLastSessionAction);
     m_historyMenu->setInitialActions(historyActions);
-#if QT_VERSION >= 0x040600 && defined(Q_WS_X11)
     m_historyRestoreLastSessionAction->setIcon(QIcon::fromTheme(QLatin1String("document-revert")));
     m_historyHomeAction->setIcon(QIcon::fromTheme(QLatin1String("go-home")));
-#endif
 
     // Bookmarks
     m_bookmarksMenu = new BookmarksMenuBarMenu(this);
@@ -777,9 +761,6 @@
 
     m_bookmarksAddAction = new QAction(this);
     m_bookmarksAddAction->setIcon(QIcon(QLatin1String(":addbookmark.png")));
-#if QT_VERSION < 0x040600 || (QT_VERSION >= 0x040600 && !defined(Q_WS_X11))
-    m_bookmarksAddAction->setIconVisibleInMenu(false);
-#endif
     connect(m_bookmarksAddAction, SIGNAL(triggered()),
             this, SLOT(addBookmark()));
     m_bookmarksAddAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
@@ -871,11 +852,9 @@
             this, SLOT(aboutApplication()));
     m_helpMenu->addAction(m_helpAboutApplicationAction);
 
-#if QT_VERSION >= 0x040600 && defined(Q_WS_X11)
     m_helpChangeLanguageAction->setIcon(QIcon::fromTheme(QLatin1String("preferences-desktop-locale")));
     m_helpAboutQtAction->setIcon(QPixmap(QLatin1String(":/trolltech/qmessagebox/images/qtlogo-64.png")));
     m_helpAboutApplicationAction->setIcon(windowIcon());
-#endif
 }
 
 void BrowserMainWindow::aboutToShowViewMenu()
