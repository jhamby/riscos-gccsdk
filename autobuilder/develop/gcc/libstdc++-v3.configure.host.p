--- libstdc++-v3/configure.host.orig	2020-07-23 07:35:19.188404867 +0100
+++ libstdc++-v3/configure.host	2020-07-24 21:08:57.500482645 +0100
@@ -264,6 +264,9 @@
   linux-musl*)
     os_include_dir="os/generic"
     ;;
+  gnueabihf)
+    os_include_dir="os/riscos"
+    ;;
   gnu* | linux* | kfreebsd*-gnu | uclinux*)
     if [ "$uclibc" = "yes" ]; then
       os_include_dir="os/uclibc"
@@ -303,6 +306,9 @@
     # Use libatomic if necessary and avoid libstdc++ specific atomicity support
     atomicity_dir="cpu/generic/atomicity_builtins"
     ;;
+  riscos)
+    os_include_dir="os/riscos"
+    ;;
   solaris2*)
     os_include_dir="os/solaris"
     ;;
@@ -360,7 +366,7 @@
 	;;
     esac
     ;;
-  arm*-*-freebsd*)
+  arm*-*-freebsd* | arm*-riscos-*)
      port_specific_symbol_files="\$(srcdir)/../config/os/gnu-linux/arm-eabi-extra.ver"
      ;;
   powerpc*-*-darwin*)
