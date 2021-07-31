--- src/ui/TabSwitcherWidget.cpp
+++ src/ui/TabSwitcherWidget.cpp
@@ -176,10 +176,14 @@ void TabSwitcherWidget::handleCurrentTabChanged(const QModelIndex &index)
 		{
 			m_loadingAnimation = new Animation(ThemesManager::getAnimationPath(QLatin1String("loading")), m_previewLabel);
 
+#ifndef __riscos__
 			connect(m_loadingAnimation, &Animation::frameChanged, [&]()
 			{
 				m_previewLabel->setPixmap(m_loadingAnimation->getCurrentPixmap());
 			});
+#else
+			connect(m_loadingAnimation, SIGNAL(frameChanged()), this, SLOT(setTabSwitcherPixmap()));
+#endif
 		}
 
 		m_loadingAnimation->start();
@@ -207,6 +211,13 @@ void TabSwitcherWidget::handleWindowAdded(quint64 identifier)
 	}
 }
 
+#ifdef __riscos__
+void TabSwitcherWidget::setTabSwitcherPixmap()
+{
+	m_previewLabel->setPixmap(m_loadingAnimation->getCurrentPixmap());
+}
+#endif
+
 void TabSwitcherWidget::handleWindowRemoved(quint64 identifier)
 {
 	const int row(findRow(identifier));


