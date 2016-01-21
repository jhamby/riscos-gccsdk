--- src/plugins/gstreamer/audiodecoder/audiodecoder.pro.orig	2016-01-01 15:04:14.174890069 +0000
+++ src/plugins/gstreamer/audiodecoder/audiodecoder.pro	2016-01-01 15:04:57.494890541 +0000
@@ -7,6 +7,11 @@
 include(../common.pri)
 
 INCLUDEPATH += $$PWD
+ INCLUDEPATH += $$PWD
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/gstreamer-0.10
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/glib-2.0
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/libxml2
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/lib/glib-2.0/include
 
 HEADERS += \
     $$PWD/qgstreameraudiodecodercontrol.h \
