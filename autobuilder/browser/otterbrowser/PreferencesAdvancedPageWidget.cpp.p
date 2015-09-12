--- src/ui/preferences/PreferencesAdvancedPageWidget.cpp.orig	2015-04-30 01:27:44.050201703 +0100
+++ src/ui/preferences/PreferencesAdvancedPageWidget.cpp	2015-04-30 01:29:32.722199292 +0100
@@ -34,7 +34,9 @@
 
 #include <QtCore/QSettings>
 #include <QtCore/QTimer>
+#ifndef __riscos__
 #include <QtMultimedia/QSoundEffect>
+#endif
 #include <QtNetwork/QSslSocket>
 #include <QtNetwork/QSslCipher>
 #include <QtWidgets/QMenu>
@@ -276,6 +278,7 @@
 
 void PreferencesAdvancedPageWidget::playNotificationSound()
 {
+#ifndef __riscos__
 	QSoundEffect *effect = new QSoundEffect(this);
 	effect->setSource(QUrl::fromLocalFile(m_ui->notificationsPlaySoundFilePathWidget->getPath()));
 	effect->setLoopCount(1);
@@ -291,6 +294,7 @@
 
 		effect->play();
 	}
+#endif
 }
 
 void PreferencesAdvancedPageWidget::updateNotificationsActions()
