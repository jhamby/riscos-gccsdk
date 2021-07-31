--- src/lib/lib.pro.orig	2016-02-21 11:14:06.926835198 +0000
+++ src/lib/lib.pro	2016-02-21 11:15:14.462835267 +0000
@@ -1,5 +1,5 @@
 isEqual(QT_MAJOR_VERSION, 5) {
-    QT += webkitwidgets network widgets printsupport sql script
+    QT += webkitwidgets network widgets printsupport sql
 } else {
     QT += core gui webkit sql network script concurrent
 }
@@ -135,8 +135,6 @@
     network/networkmanager.cpp \
     network/networkmanagerproxy.cpp \
     network/networkproxyfactory.cpp \
-    network/pac/pacmanager.cpp \
-    network/pac/proxyautoconfig.cpp \
     network/schemehandlers/adblockschemehandler.cpp \
     network/schemehandlers/fileschemehandler.cpp \
     network/schemehandlers/ftpschemehandler.cpp \
@@ -221,7 +219,6 @@
     tools/html5permissions/html5permissionsnotification.cpp \
     tools/iconfetcher.cpp \
     tools/iconprovider.cpp \
-    tools/json.cpp \
     tools/listitemdelegate.cpp \
     tools/mactoolbutton.cpp \
     tools/menubar.cpp \
@@ -335,9 +332,6 @@
     network/networkmanager.h \
     network/networkmanagerproxy.h \
     network/networkproxyfactory.h \
-    network/pac/pacdatetime.h \
-    network/pac/pacmanager.h \
-    network/pac/proxyautoconfig.h \
     network/schemehandlers/adblockschemehandler.h \
     network/schemehandlers/fileschemehandler.h \
     network/schemehandlers/ftpschemehandler.h \
@@ -425,7 +419,6 @@
     tools/html5permissions/html5permissionsnotification.h \
     tools/iconfetcher.h \
     tools/iconprovider.h \
-    tools/json.h \
     tools/listitemdelegate.h \
     tools/mactoolbutton.h \
     tools/menubar.h \
