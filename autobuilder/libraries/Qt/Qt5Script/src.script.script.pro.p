--- src/script/script.pro.orig	2015-11-30 16:33:32.000000000 +0000
+++ src/script/script.pro	2015-11-30 16:33:50.483529778 +0000
@@ -76,3 +76,8 @@
 }
 
 TR_EXCLUDE = $$WEBKITDIR/*
+
+arm-riscos-vfp-g++ {
+# Start the path with a '/' to ensure QMake knows it's a rooted path.
+    target.path = /$(GCCSDK_INSTALL_ENV)/vfp/lib
+}
