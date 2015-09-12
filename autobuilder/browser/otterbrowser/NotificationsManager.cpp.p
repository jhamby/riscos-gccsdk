--- ./src/core/NotificationsManager.cpp.orig	2015-04-30 00:24:39.298113383 +0100
+++ ./src/core/NotificationsManager.cpp	2015-04-30 00:26:30.022112181 +0100
@@ -27,7 +27,9 @@
 #include <QtCore/QMetaEnum>
 #include <QtCore/QSettings>
 #include <QtCore/QTimer>
+#ifndef __riscos__
 #include <QtMultimedia/QSoundEffect>
+#endif
 
 namespace Otter
 {
@@ -86,6 +88,7 @@
 	Notification *notification = new Notification(message, level, event, Application::getInstance());
 	const EventDefinition definition = getEventDefinition(event);
 
+#ifndef __riscos__
 	if (!definition.playSound.isEmpty())
 	{
 		QSoundEffect *effect = new QSoundEffect(m_instance);
@@ -104,6 +107,7 @@
 			effect->play();
 		}
 	}
+#endif
 
 	if (definition.showAlert)
 	{
@@ -147,7 +151,9 @@
 	const QSettings notificationsSettings(SessionsManager::getReadableDataPath(QLatin1String("notifications.ini")), QSettings::IniFormat);
 	const QString eventName = getEventName(identifier);
 
+#ifndef __riscos__
 	m_definitions[identifier].playSound = notificationsSettings.value(eventName + QLatin1String("/playSound"), QString()).toString();
+#endif
 	m_definitions[identifier].showAlert = notificationsSettings.value(eventName + QLatin1String("/showAlert"), false).toBool();
 	m_definitions[identifier].showNotification = notificationsSettings.value(eventName + QLatin1String("/showNotification"), false).toBool();
 
@@ -162,7 +168,9 @@
 	{
 		notificationsSettings.beginGroup(getEventName(i));
 
+#ifndef __riscos__
 		m_definitions[i].playSound = notificationsSettings.value(QLatin1String("playSound")).toString();
+#endif
 		m_definitions[i].showAlert = notificationsSettings.value(QLatin1String("showAlert")).toBool();
 		m_definitions[i].showNotification = notificationsSettings.value(QLatin1String("showNotification")).toBool();
 
