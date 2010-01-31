--- gcc/config.host.orig	2008-05-11 13:06:19.000000000 +0200
+++ gcc/config.host	2008-05-11 19:26:16.000000000 +0200
@@ -107,6 +107,9 @@
     prefix=/gnu
     local_prefix=/gnu
     ;;
+  arm*-*-riscos)	# ARM RISC OS with ELF
+    host_xm_file="arm/xm-riscos.h"
+    ;;
   hppa1.1-*-pro*)
     host_xmake_file="pa/x-ada"
     ;;
