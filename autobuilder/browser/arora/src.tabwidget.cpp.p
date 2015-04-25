--- src/tabwidget.cpp.orig	2015-04-10 20:47:29.384351907 +0100
+++ src/tabwidget.cpp	2015-04-11 14:38:43.990006977 +0100
@@ -7,7 +7,7 @@
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
- * but WITHOUT ANY WARRANTY; without even the iQ_WS_X11mplied warranty of
+ * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
@@ -139,28 +139,20 @@
     m_closeTabAction = new QAction(this);
     m_closeTabAction->setShortcuts(QKeySequence::Close);
     m_closeTabAction->setIcon(QIcon(QLatin1String(":graphics/closetab.png")));
-#if QT_VERSION < 0x040600 || (QT_VERSION >= 0x040600 && !defined(Q_WS_X11))
-    m_closeTabAction->setIconVisibleInMenu(false);
-#endif
     connect(m_closeTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));
 
     m_bookmarkTabsAction = new QAction(this);
     connect(m_bookmarkTabsAction, SIGNAL(triggered()), this, SLOT(bookmarkTabs()));
 
     m_newTabAction->setIcon(QIcon(QLatin1String(":graphics/addtab.png")));
-#if QT_VERSION < 0x040600 || (QT_VERSION >= 0x040600 && !defined(Q_WS_X11))
-    m_newTabAction->setIconVisibleInMenu(false);
-#endif
 
     m_nextTabAction = new QAction(this);
     connect(m_nextTabAction, SIGNAL(triggered()), this, SLOT(nextTab()));
 
     m_previousTabAction = new QAction(this);
     connect(m_previousTabAction, SIGNAL(triggered()), this, SLOT(previousTab()));
-#if QT_VERSION >= 0x040600 && defined(Q_WS_X11)
     m_previousTabAction->setIcon(QIcon::fromTheme(QLatin1String("go-previous")));
     m_nextTabAction->setIcon(QIcon::fromTheme(QLatin1String("go-next")));
-#endif
 
     m_recentlyClosedTabsMenu = new QMenu(this);
     connect(m_recentlyClosedTabsMenu, SIGNAL(aboutToShow()),
