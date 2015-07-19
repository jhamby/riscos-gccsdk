--- src/platformsupport/platformsupport.pro.orig	2015-06-29 21:04:38.000000000 +0100
+++ src/platformsupport/platformsupport.pro	2015-07-15 18:24:19.930735896 +0100
@@ -27,4 +27,6 @@
     include(dbustray/dbustray.pri)
 }
 
+QMAKE_CXXFLAGS += -fPIC
+
 load(qt_module)
