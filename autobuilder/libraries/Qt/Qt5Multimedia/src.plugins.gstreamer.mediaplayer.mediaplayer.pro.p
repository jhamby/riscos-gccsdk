--- src/plugins/gstreamer/mediaplayer/mediaplayer.pro.orig	2016-01-01 14:50:42.886881232 +0000
+++ src/plugins/gstreamer/mediaplayer/mediaplayer.pro	2016-01-01 14:51:17.426881609 +0000
@@ -7,6 +7,10 @@
 include(../common.pri)
 
 INCLUDEPATH += $$PWD
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/gstreamer-0.10
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/glib-2.0
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/libxml2
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/lib/glib-2.0/include
 
 HEADERS += \
     $$PWD/qgstreamerplayercontrol.h \
