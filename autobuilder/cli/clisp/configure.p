--- configure_base	2006-07-01 09:11:49.000000000 +0100
+++ configure	2006-08-13 13:01:40.000000000 +0100
@@ -448,6 +448,8 @@
 
 conf_dir () {
   cd $1; shift;
+  echo "patching `pwd`/configure $*"
+  patch -p0 <"$ABS_SRCDIR"/ff8_patch
   echo "executing `pwd`/configure $*"
   ${CONFIG_SHELL-/bin/sh} configure $*;
   cd ..;
