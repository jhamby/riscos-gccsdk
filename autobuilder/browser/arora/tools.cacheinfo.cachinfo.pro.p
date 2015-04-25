--- tools/cacheinfo/cacheinfo.pro.orig	2010-09-27 03:42:17.000000000 +0100
+++ tools/cacheinfo/cacheinfo.pro	2015-04-24 15:09:44.999655721 +0100
@@ -18,6 +18,9 @@
 
 include(../../install.pri)
 
+# To support both Qt4 and Qt5
+DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
+
 !mac {
 unix {
     INSTALLS += man man-compress
