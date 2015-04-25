--- src/browserapplication.cpp.orig	2010-09-27 03:42:17.000000000 +0100
+++ src/browserapplication.cpp	2015-04-23 20:53:07.272163475 +0100
@@ -111,6 +111,7 @@
 #endif
     ));
 
+#ifndef Q_OS_RISCOS
 #ifndef AUTOTESTS
     connect(this, SIGNAL(messageReceived(QLocalSocket *)),
             this, SLOT(messageReceived(QLocalSocket *)));
@@ -133,6 +134,7 @@
     if (!startSingleServer())
         return;
 #endif
+#endif
 
 #if defined(Q_WS_MAC)
     QApplication::setQuitOnLastWindowClosed(false);
@@ -648,7 +650,7 @@
 
 QString BrowserApplication::installedDataDirectory()
 {
-#if defined(Q_WS_X11)
+#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
     return QLatin1String(PKGDATADIR);
 #else
     return qApp->applicationDirPath();
