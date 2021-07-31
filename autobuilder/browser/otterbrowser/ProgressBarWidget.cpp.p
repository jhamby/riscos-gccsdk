--- src/modules/windows/web/ProgressBarWidget.cpp
+++ src/modules/windows/web/ProgressBarWidget.cpp
@@ -52,7 +52,7 @@ ProgressBarWidget::ProgressBarWidget(Window *window, WebWidget *parent) : QFrame
 	updateLoadingState(window->getLoadingState());
 	setAutoFillBackground(true);
 
-	connect(window, &Window::loadingStateChanged, this, &ProgressBarWidget::updateLoadingState);
+	connect(window, SIGNAL(loadingStateChanged(WebWidget::LoadingState)), this, SLOT(updateLoadingState(WebWidget::LoadingState)));
 }
 
 void ProgressBarWidget::timerEvent(QTimerEvent *event)
@@ -76,7 +76,7 @@ void ProgressBarWidget::timerEvent(QTimerEvent *event)
 
 			if (!isVisible())
 			{
-				connect(m_webWidget, &WebWidget::geometryChanged, this, &ProgressBarWidget::scheduleGeometryUpdate);
+				connect(m_webWidget, SIGNAL(geometryChanged()), this, SLOT(scheduleGeometryUpdate()));
 			}
 
 			if (!geometry.isValid())
@@ -93,7 +93,7 @@ void ProgressBarWidget::timerEvent(QTimerEvent *event)
 		}
 		else
 		{
-			disconnect(m_webWidget, &WebWidget::geometryChanged, this, &ProgressBarWidget::scheduleGeometryUpdate);
+			disconnect(m_webWidget, SIGNAL(geometryChanged()), this, SLOT(scheduleGeometryUpdate()));
 
 			hide();
 		}
@@ -114,7 +114,7 @@ void ProgressBarWidget::updateLoadingState(WebWidget::LoadingState state)
 
 		if (m_window && m_webWidget)
 		{
-			disconnect(m_webWidget, &WebWidget::geometryChanged, this, &ProgressBarWidget::scheduleGeometryUpdate);
+			disconnect(m_webWidget, SIGNAL(geometryChanged()), this, SLOT(scheduleGeometryUpdate()));
 		}
 	}
 }


