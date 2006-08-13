--- configure.orig	2005-07-03 21:57:21.160000000 +0100
+++ configure	2005-07-03 21:57:11.100000000 +0100
@@ -7692,6 +7692,11 @@
   dynamic_linker=no
   ;;
 
+*riscos*)
+  dynamic_linker=no
+  version_type=linux
+  ;;
+
 kfreebsd*-gnu)
   version_type=linux
   need_lib_prefix=no
