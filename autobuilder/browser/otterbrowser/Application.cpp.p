--- src/core/Application.cpp.orig	2016-09-07 08:13:14.737610559 +0100
+++ src/core/Application.cpp	2016-09-07 08:28:48.289611514 +0100
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
@@ -369,9 +371,12 @@
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
@@ -393,7 +398,7 @@
 
 		LongTermTimer::runTimer((interval * SECONDS_IN_DAY), this, SLOT(periodicUpdateCheck()));
 	}
-
+#endif
 	setStyle(SettingsManager::getValue(SettingsManager::Interface_WidgetStyleOption).toString());
 
 	const QString styleSheet(SettingsManager::getValue(SettingsManager::Interface_StyleSheetOption).toString());
