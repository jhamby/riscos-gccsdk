--- src/plugins/gstreamer/gstreamer.pro.orig	2016-01-01 14:55:57.510884659 +0000
+++ src/plugins/gstreamer/gstreamer.pro	2016-01-01 14:58:07.218886072 +0000
@@ -1,13 +1,13 @@
 TEMPLATE = subdirs
 
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/gstreamer-0.10
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/glib-2.0
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/libxml2
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/lib/glib-2.0/include
+
 SUBDIRS += \
     audiodecoder \
-    mediaplayer \
-    mediacapture
-
-config_gstreamer_encodingprofiles {
-    SUBDIRS += camerabin
-}
+    mediaplayer
 
 OTHER_FILES += \
     gstreamer.json
