--- src/core/ActionsManager.cpp.orig	2017-04-13 19:11:44.073768365 +0100
+++ src/core/ActionsManager.cpp	2017-04-13 19:15:01.250000000 +0100
@@ -208,7 +208,9 @@
 	registerAction(WebsiteInformationAction, QT_TRANSLATE_NOOP("actions", "Website Information…"), QString(), QIcon(), ActionDefinition::WindowScope);
 	registerAction(WebsiteCertificateInformationAction, QT_TRANSLATE_NOOP("actions", "Website Certificate Information…"), QString(), QIcon(), ActionDefinition::WindowScope);
 	registerAction(SwitchApplicationLanguageAction, QT_TRANSLATE_NOOP("actions", "Switch Application Language…"), QString(), ThemesManager::getIcon(QLatin1String("preferences-desktop-locale")), ActionDefinition::ApplicationScope);
+#ifndef __riscos__
 	registerAction(CheckForUpdatesAction, QT_TRANSLATE_NOOP("actions", "Check for Updates…"), QString(), QIcon(), ActionDefinition::ApplicationScope);
+#endif
 	registerAction(DiagnosticReportAction, QT_TRANSLATE_NOOP("actions", "Diagnostic Report…"), QString(), QIcon(), ActionDefinition::ApplicationScope);
 	registerAction(AboutApplicationAction, QT_TRANSLATE_NOOP("actions", "About Otter…"), QString(), ThemesManager::getIcon(QLatin1String("otter-browser")), ActionDefinition::ApplicationScope);
 	registerAction(AboutQtAction, QT_TRANSLATE_NOOP("actions", "About Qt…"), QString(), ThemesManager::getIcon(QLatin1String("qt")), ActionDefinition::ApplicationScope);
