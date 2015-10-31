--- src/core/ActionsManager.cpp.orig	2015-10-31 12:38:52.674864122 +0000
+++ src/core/ActionsManager.cpp	2015-10-31 12:40:57.678861347 +0000
@@ -424,7 +424,9 @@
 	registerAction(QuickPreferencesAction, QT_TRANSLATE_NOOP("actions", "Quick Preferences"));
 	registerAction(ResetQuickPreferencesAction, QT_TRANSLATE_NOOP("actions", "Reset Options"));
 	registerAction(SwitchApplicationLanguageAction, QT_TRANSLATE_NOOP("actions", "Switch Application Language…"), QString(), Utils::getIcon(QLatin1String("preferences-desktop-locale")));
+#ifndef __riscos__
 	registerAction(CheckForUpdatesAction, QT_TRANSLATE_NOOP("actions", "Check for Updates…"));
+#endif
 	registerAction(DiagnosticReportAction, QT_TRANSLATE_NOOP("actions", "Diagnostic Report…"));
 	registerAction(AboutApplicationAction, QT_TRANSLATE_NOOP("actions", "About Otter…"), QString(), Utils::getIcon(QLatin1String("otter-browser"), false));
 	registerAction(AboutQtAction, QT_TRANSLATE_NOOP("actions", "About Qt…"), QString(), Utils::getIcon(QLatin1String("qt"), false));
