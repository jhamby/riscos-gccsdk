--- config.tests/gstreamer/gstreamer.pro.orig	2016-01-01 14:25:15.770864599 +0000
+++ config.tests/gstreamer/gstreamer.pro	2016-01-01 14:25:36.034864819 +0000
@@ -1,6 +1,10 @@
 SOURCES += main.cpp
 
 CONFIG += link_pkgconfig
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/gstreamer-0.10
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/glib-2.0
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/libxml2
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/lib/glib-2.0/include
 
 PKGCONFIG += \
     gstreamer-$$GST_VERSION \
