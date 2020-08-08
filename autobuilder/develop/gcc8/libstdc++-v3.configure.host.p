--- libstdc++-v3/configure.host.orig	2017-02-02 16:24:03.000000000 +0000
+++ libstdc++-v3/configure.host	2018-06-22 11:26:51.090539259 +0100
@@ -261,6 +261,9 @@
   linux-musl*)
     os_include_dir="os/generic"
     ;;
+  gnueabihf)
+    os_include_dir="os/riscos"
+    ;;
   gnu* | linux* | kfreebsd*-gnu)
     if [ "$uclibc" = "yes" ]; then
       os_include_dir="os/uclibc"
@@ -300,6 +303,9 @@
     # Use libatomic if necessary and avoid libstdc++ specific atomicity support
     atomicity_dir="cpu/generic/atomicity_builtins"
     ;;
+  riscos)
+    os_include_dir="os/riscos"
+    ;;
   solaris2)
     # This too-vague configuration does not provide enough information
     # to select a ctype include, and thus os_include_dir is a crap shoot.
@@ -360,7 +366,7 @@
 	;;
     esac
     ;;
-  arm*-*-freebsd*)
+  arm*-*-freebsd* | arm*-riscos-*)
      port_specific_symbol_files="\$(srcdir)/../config/os/gnu-linux/arm-eabi-extra.ver"
      ;;
   powerpc*-*-darwin*)
