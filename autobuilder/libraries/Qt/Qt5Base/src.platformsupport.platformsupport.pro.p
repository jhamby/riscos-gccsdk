--- src/platformsupport/platformsupport.pro.orig	2014-05-15 18:12:11.000000000 +0100
+++ src/platformsupport/platformsupport.pro	2014-05-27 19:55:16.358522842 +0100
@@ -21,4 +21,6 @@
 include(linuxaccessibility/linuxaccessibility.pri)
 include(clipboard/clipboard.pri)
 
+QMAKE_CXXFLAGS += -fPIC
+
 load(qt_module)
