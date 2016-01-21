--- config.tests/gstreamer_appsrc/gstreamer_appsrc.pro.orig	2016-01-01 15:22:58.486902315 +0000
+++ config.tests/gstreamer_appsrc/gstreamer_appsrc.pro	2016-01-01 15:23:30.678902666 +0000
@@ -2,6 +2,11 @@
 
 CONFIG += link_pkgconfig
 
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/gstreamer-0.10
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/glib-2.0
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/libxml2
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/lib/glib-2.0/include
+
 PKGCONFIG += \
     gstreamer-$$GST_VERSION \
     gstreamer-base-$$GST_VERSION \
