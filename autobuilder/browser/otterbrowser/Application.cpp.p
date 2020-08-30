--- src/core/Application.cpp
+++ src/core/Application.cpp
@@ -46,8 +46,10 @@
 #elif defined(Q_OS_MAC)
 #include "../modules/platforms/mac/MacPlatformIntegration.h"
 #elif defined(Q_OS_UNIX)
+#if !defined(Q_OS_RISCOS)
 #include "../modules/platforms/freedesktoporg/FreeDesktopOrgPlatformIntegration.h"
 #endif
+#endif
 #include "../ui/LocaleDialog.h"
 #include "../ui/MainWindow.h"
 #include "../ui/NotificationDialog.h"
@@ -442,9 +444,12 @@ Application::Application(int &argc, char **argv) : QApplication(argc, argv), Act
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
@@ -454,6 +459,7 @@ Application::Application(int &argc, char **argv) : QApplication(argc, argv), Act
 			return;
 		}
 	}
+#endif
 
 	const QDate lastUpdate(QDate::fromString(SettingsManager::getOption(SettingsManager::Updates_LastCheckOption).toString(), Qt::ISODate));
 	const int interval(SettingsManager::getOption(SettingsManager::Updates_CheckIntervalOption).toInt());
@@ -490,9 +496,8 @@ Application::Application(int &argc, char **argv) : QApplication(argc, argv), Act
 	QDesktopServices::setUrlHandler(QLatin1String("http"), this, "openUrl");
 	QDesktopServices::setUrlHandler(QLatin1String("https"), this, "openUrl");
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &Application::handleOptionChanged);
-	connect(this, &Application::aboutToQuit, this, &Application::handleAboutToQuit);
-	connect(this, &Application::focusObjectChanged, this, &Application::handleFocusObjectChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int,QVariant)));
+	connect(this, SIGNAL(aboutToQuit()), this, SLOT(handleAboutToQuit()));
 }
 
 Application::~Application()
@@ -654,6 +659,7 @@ void Application::triggerAction(int identifier, const QVariantMap &parameters, Q
 			}
 
 			return;
+#ifndef __riscos__
 		case ActionsManager::CheckForUpdatesAction:
 			{
 				UpdateCheckerDialog *dialog(new UpdateCheckerDialog(m_activeWindow));
@@ -670,6 +676,7 @@ void Application::triggerAction(int identifier, const QVariantMap &parameters, Q
 			}
 
 			return;
+#endif
 		case ActionsManager::AboutApplicationAction:
 			{
 				WebBackend *webBackend(AddonsManager::getWebBackend());
@@ -968,6 +975,15 @@ void Application::handleFocusObjectChanged(QObject *object)
 	}
 }
 
+#ifdef __riscos__
+
+void Application::setActiveWindow(MainWindow *window)
+{
+	m_activeWindow = window;
+}
+
+#endif
+
 void Application::handlePositionalArguments(QCommandLineParser *parser, bool forceOpen)
 {
 	SessionsManager::OpenHints openHints(SessionsManager::DefaultOpen);
@@ -1182,10 +1198,14 @@ MainWindow* Application::createWindow(const QVariantMap &parameters, const Sessi
 
 	emit m_instance->windowAdded(window);
 
+#ifndef __riscos__
 	connect(window, &MainWindow::activated, [&](MainWindow *window)
 	{
 		m_activeWindow = window;
 	});
+#else
+	connect(window, SIGNAL(activated(MainWindow*)), m_instance, SLOT(setActiveWindow(MainWindow*)));
+#endif
 
 	return window;
 }


