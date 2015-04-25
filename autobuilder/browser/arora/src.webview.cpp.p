--- src/webview.cpp.orig	2015-04-10 20:47:29.384351907 +0100
+++ src/webview.cpp	2015-04-11 14:04:39.681980485 +0100
@@ -83,20 +83,21 @@
 #include <qdebug.h>
 #include <qevent.h>
 #include <qmenubar.h>
+#include <qmimedata.h>
 #include <qtimer.h>
 #include <qwebframe.h>
 
 #if QT_VERSION >= 0x040600 || defined(WEBKIT_TRUNK)
-#if !defined(QTWEBKIT_VERSION) || QTWEBKIT_VERSION < 0x020000
+#if QT_VERSION < 0x040700
 Q_DECLARE_METATYPE(QWebElement)
 #endif
 #include <qinputdialog.h>
 #include <qlabel.h>
 #include <qmessagebox.h>
 #include <qsettings.h>
+#include <qstylefactory.h>
 #include <qtooltip.h>
 #include <qwebelement.h>
-#include <qwindowsstyle.h>
 #endif
 
 #include <qdebug.h>
@@ -115,8 +116,8 @@
 #if QT_VERSION >= 0x040600
     QPalette p;
     if (p.color(QPalette::Window) != Qt::white) {
-        QWindowsStyle s;
-        p = s.standardPalette();
+        QScopedPointer<QStyle> s(QStyleFactory::create(QLatin1String("windows")));
+        p = s->standardPalette();
         setPalette(p);
     }
 #endif
