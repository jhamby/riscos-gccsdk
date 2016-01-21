--- src/gsttools/gsttools.pro.orig	2016-01-01 14:41:37.702875294 +0000
+++ src/gsttools/gsttools.pro	2016-01-01 14:42:27.314875834 +0000
@@ -3,6 +3,11 @@
 TARGET = qgsttools_p
 QPRO_PWD = $$PWD
 
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/gstreamer-0.10
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/glib-2.0
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/libxml2
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/lib/glib-2.0/include
+
 QT = core-private multimedia-private gui-private
 
 !static:DEFINES += QT_MAKEDLL
