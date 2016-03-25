--- src/core/ActionsManager.cpp.orig	2016-03-25 08:41:26.850083108 +0000
+++ src/core/ActionsManager.cpp	2016-03-25 08:43:52.782084697 +0000
@@ -442,7 +442,9 @@
 	registerAction(WebsiteInformationAction, QT_TRANSLATE_NOOP("actions", "Website Information…"));
 	registerAction(WebsiteCertificateInformationAction, QT_TRANSLATE_NOOP("actions", "Website Certificate Information…"));
 	registerAction(SwitchApplicationLanguageAction, QT_TRANSLATE_NOOP("actions", "Switch Application Language…"), QString(), ThemesManager::getIcon(QLatin1String("preferences-desktop-locale")));
+#ifndef __riscos__
 	registerAction(CheckForUpdatesAction, QT_TRANSLATE_NOOP("actions", "Check for Updates…"));
+#endif
 	registerAction(DiagnosticReportAction, QT_TRANSLATE_NOOP("actions", "Diagnostic Report…"));
 	registerAction(AboutApplicationAction, QT_TRANSLATE_NOOP("actions", "About Otter…"), QString(), ThemesManager::getIcon(QLatin1String("otter-browser"), NoFlags));
 	registerAction(AboutQtAction, QT_TRANSLATE_NOOP("actions", "About Qt…"), QString(), ThemesManager::getIcon(QLatin1String("qt"), NoFlags));
