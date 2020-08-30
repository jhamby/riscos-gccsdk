--- src/core/ActionsManager.cpp
+++ src/core/ActionsManager.cpp
@@ -451,13 +451,15 @@ ActionsManager::ActionsManager(QObject *parent) : QObject(parent),
 	registerAction(WebsiteInformationAction, QT_TRANSLATE_NOOP("actions", "Website Information…"), QString(), QIcon(), ActionDefinition::WindowScope);
 	registerAction(WebsiteCertificateInformationAction, QT_TRANSLATE_NOOP("actions", "Website Certificate Information…"), QString(), QIcon(), ActionDefinition::WindowScope);
 	registerAction(SwitchApplicationLanguageAction, QT_TRANSLATE_NOOP("actions", "Switch Application Language…"), QString(), ThemesManager::createIcon(QLatin1String("preferences-desktop-locale")), ActionDefinition::ApplicationScope, (ActionDefinition::IsEnabledFlag | ActionDefinition::IsImmutableFlag));
+#ifndef __riscos__
 	registerAction(CheckForUpdatesAction, QT_TRANSLATE_NOOP("actions", "Check for Updates…"), QString(), QIcon(), ActionDefinition::ApplicationScope, (ActionDefinition::IsEnabledFlag | ActionDefinition::IsImmutableFlag));
+#endif
 	registerAction(DiagnosticReportAction, QT_TRANSLATE_NOOP("actions", "Diagnostic Report…"), QString(), QIcon(), ActionDefinition::ApplicationScope, (ActionDefinition::IsEnabledFlag | ActionDefinition::IsImmutableFlag));
 	registerAction(AboutApplicationAction, QT_TRANSLATE_NOOP("actions", "About Otter…"), QString(), ThemesManager::createIcon(QLatin1String("otter-browser"), false), ActionDefinition::ApplicationScope, (ActionDefinition::IsEnabledFlag | ActionDefinition::IsImmutableFlag));
 	registerAction(AboutQtAction, QT_TRANSLATE_NOOP("actions", "About Qt…"), QString(), ThemesManager::createIcon(QLatin1String("qt")), ActionDefinition::ApplicationScope, (ActionDefinition::IsEnabledFlag | ActionDefinition::IsImmutableFlag));
 	registerAction(ExitAction, QT_TRANSLATE_NOOP("actions", "Exit"), QString(), ThemesManager::createIcon(QLatin1String("application-exit")), ActionDefinition::ApplicationScope, (ActionDefinition::IsEnabledFlag | ActionDefinition::IsImmutableFlag));
 
-	connect(SettingsManager::getInstance(), &SettingsManager::optionChanged, this, &ActionsManager::handleOptionChanged);
+	connect(SettingsManager::getInstance(), SIGNAL(optionChanged(int,QVariant)), this, SLOT(handleOptionChanged(int)));
 }
 
 void ActionsManager::createInstance()


