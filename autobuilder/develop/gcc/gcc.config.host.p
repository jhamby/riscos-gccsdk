--- gcc/config.host.orig	2020-07-23 07:35:17.316384243 +0100
+++ gcc/config.host	2020-07-24 21:53:51.529434564 +0100
@@ -107,7 +107,7 @@
 	;;
     esac
     ;;
-  arm*-*-freebsd* | arm*-*-netbsd* | arm*-*-linux* | arm*-*-fuchsia*)
+  arm*-*-freebsd* | arm*-*-netbsd* | arm*-*-linux* | arm*-*-fuchsia* | arm*-riscos*)
     case ${target} in
       arm*-*-*)
 	host_extra_gcc_objs="driver-arm.o"
@@ -200,6 +200,9 @@
 
 # Machine-specific settings.
 case ${host} in
+  arm*-*-riscos | arm*-riscos*)	# ARM RISC OS with ELF
+    host_xm_file="arm/xm-riscos.h"
+    ;;
   hppa1.0-*-hpux10* | hppa1.1-*-hpux10* | hppa2*-*-hpux10*)
     out_host_hook_obj=host-hpux.o
     host_xmake_file="${host_xmake_file} x-hpux"
