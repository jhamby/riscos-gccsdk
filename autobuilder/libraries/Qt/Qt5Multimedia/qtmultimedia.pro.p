--- qtmultimedia.pro.orig	2016-01-01 14:18:50.538860403 +0000
+++ qtmultimedia.pro	2016-01-01 14:20:54.486861753 +0000
@@ -1,5 +1,7 @@
 requires(qtHaveModule(gui))
 
+INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include/gstreamer-0.10
+
 load(configure)
 qtCompileTest(openal)
 win32 {
