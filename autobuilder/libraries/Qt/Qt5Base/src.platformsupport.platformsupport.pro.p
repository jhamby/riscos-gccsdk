--- src/platformsupport/platformsupport.pro.orig	2015-02-17 04:56:38.000000000 +0000
+++ src/platformsupport/platformsupport.pro	2015-04-12 13:43:51.604657031 +0100
@@ -22,4 +22,6 @@
 include(linuxaccessibility/linuxaccessibility.pri)
 include(clipboard/clipboard.pri)
 
+QMAKE_CXXFLAGS += -fPIC
+
 load(qt_module)
