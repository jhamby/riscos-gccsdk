--- src/scripttools/scripttools.pro.orig	2015-11-30 16:39:14.000000000 +0000
+++ src/scripttools/scripttools.pro	2015-11-30 16:39:40.447534313 +0000
@@ -13,3 +13,8 @@
 load(qt_module)
 
 include(debugging/debugging.pri)
+
+arm-riscos-vfp-g++ {
+# Start the path with a '/' to ensure QMake knows it's a rooted path.
+    target.path = /$(GCCSDK_INSTALL_ENV)/vfp/lib
+}
