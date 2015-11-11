--- src/core/Application.cpp.orig	2015-11-11 19:46:35.590937053 +0000
+++ src/core/Application.cpp	2015-11-11 19:49:58.734932544 +0000
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
 #include "../ui/ReportDialog.h"
@@ -285,9 +287,12 @@
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
@@ -309,7 +314,7 @@
 
 		LongTermTimer::runTimer((interval * SECONDS_IN_DAY), this, SLOT(periodicUpdateCheck()));
 	}
-
+#endif
 	setStyle(SettingsManager::getValue(QLatin1String("Interface/WidgetStyle")).toString());
 
 	const QString styleSheet = SettingsManager::getValue(QLatin1String("Interface/StyleSheet")).toString();
