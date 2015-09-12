--- src/core/Application.cpp.orig	2015-09-12 12:39:52.234018708 +0100
+++ src/core/Application.cpp	2015-09-12 13:27:15.777955597 +0100
@@ -44,8 +44,10 @@
 #elif defined(Q_OS_MAC)
 #include "../modules/platforms/mac/MacPlatformIntegration.h"
 #elif defined(Q_OS_UNIX)
+#if !defined(Q_OS_RISCOS)
 #include "../modules/platforms/freedesktoporg/FreeDesktopOrgPlatformIntegration.h"
 #endif
+#endif
 #include "../ui/MainWindow.h"
 #include "../ui/NotificationDialog.h"
 #include "../ui/TrayIcon.h"
@@ -282,9 +284,12 @@
 #elif defined(Q_OS_MAC)
 	m_platformIntegration = new MacPlatformIntegration(this);
 #elif defined(Q_OS_UNIX)
+#if !defined(Q_OS_RISCOS)
 	m_platformIntegration = new FreeDesktopOrgPlatformIntegration(this);
 #endif
+#endif
 
+#if !defined(Q_OS_RISCOS)
 	if (Updater::isReadyToInstall())
 	{
 		m_isUpdating = Updater::installUpdate();
@@ -306,7 +311,7 @@
 
 		LongTermTimer::runTimer((interval * SECONDS_IN_DAY), this, SLOT(periodicUpdateCheck()));
 	}
-
+#endif
 	connect(this, SIGNAL(aboutToQuit()), this, SLOT(clearHistory()));
 }
 
