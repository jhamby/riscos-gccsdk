--- configure.orig	2007-11-15 12:39:35.558607700 +0000
+++ configure	2007-11-15 12:40:24.466607700 +0000
@@ -13,6 +13,9 @@
 ## M4sh Initialization.  ##
 ## --------------------- ##
 
+# Use libtool suitable for cross-compiling
+ac_cv_path_LTOOL=`pwd`/libtool
+
 # Be more Bourne compatible
 DUALCASE=1; export DUALCASE # for MKS sh
 if test -n "${ZSH_VERSION+set}" && (emulate sh) >/dev/null 2>&1; then
