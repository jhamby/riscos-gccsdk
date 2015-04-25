--- src/src.pri.orig	2010-09-27 03:42:17.000000000 +0100
+++ src/src.pri	2015-04-24 20:01:57.579882917 +0100
@@ -7,7 +7,31 @@
 INCLUDEPATH += $$PWD
 DEPENDPATH += $$PWD
 
-QT += webkit network
+VPATH = \
+    $$PWD/adblock \
+    $$PWD/bookmarks \
+    $$PWD/bookmarks/xbel \
+    $$PWD/data \
+    $$PWD/history \
+    $$PWD/htmls \
+    $$PWD/locale \
+    $$PWD/locationbar \
+    $$PWD/network \
+    $$PWD/network/cookiejar \
+    $$PWD/network/cookiejar/networkcookiejar \
+    $$PWD/opensearch \
+    $$PWD/qwebplugins \
+    $$PWD/qwebplugins/clicktoflash \
+    $$PWD/useragent \
+    $$PWD/utils \
+    $$PWD
+
+QT += network
+contains(QT_VERSION, ^5.*) {
+    QT += widgets webkitwidgets printsupport
+} else {
+    QT += webkit
+}
 
 # Share object files for faster compiling
 RCC_DIR     = $$PWD/.rcc
@@ -127,6 +151,9 @@
 
 include(../webkittrunk.pri)
 
+# To support both Qt4 and Qt5
+DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
+
 unix {
     PKGDATADIR = $$DATADIR/arora
     DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"
@@ -135,3 +162,8 @@
 win32 {
     LIBS += -ladvapi32
 }
+
+*riscos* {
+    QMAKE_LFLAGS = -Wl,-rpath-link,$(WEBKITOUTPUTDIR)/Release/lib
+    LIBS += -L$(WEBKITOUTPUTDIR)/Release/lib -lWebKit1
+}
