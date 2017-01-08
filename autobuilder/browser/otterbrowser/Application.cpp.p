--- src/core/Application.cpp.orig	2016-12-14 21:15:37.934568977 +0000
+++ src/core/Application.cpp	2016-12-14 21:23:55.980197347 +0000
@@ -46,8 +46,10 @@
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
@@ -414,9 +416,12 @@
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
@@ -438,6 +443,7 @@
 
 		LongTermTimer::runTimer((interval * SECONDS_IN_DAY), this, SLOT(periodicUpdateCheck()));
 	}
+#endif
 
 	setStyle(ThemesManager::createStyle(SettingsManager::getValue(SettingsManager::Interface_WidgetStyleOption).toString()));
 
