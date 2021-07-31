--- gcc/config.host.orig	2018-01-03 10:03:58.000000000 +0000
+++ gcc/config.host	2018-05-17 19:40:06.765301270 +0100
@@ -107,7 +107,7 @@
 	;;
     esac
     ;;
-  arm*-*-freebsd* | arm*-*-linux* | arm*-*-fuchsia*)
+  arm*-*-freebsd* | arm*-*-linux* | arm*-*-fuchsia* | arm*-riscos*)
     case ${target} in
       arm*-*-*)
 	host_extra_gcc_objs="driver-arm.o"
@@ -204,6 +204,9 @@
 
 # Machine-specific settings.
 case ${host} in
+  arm*-*-riscos | arm*-riscos*)	# ARM RISC OS with ELF
+    host_xm_file="arm/xm-riscos.h"
+    ;;
   hppa1.0-*-hpux10* | hppa1.1-*-hpux10* | hppa2*-*-hpux10*)
     out_host_hook_obj=host-hpux.o
     host_xmake_file="${host_xmake_file} x-hpux"
