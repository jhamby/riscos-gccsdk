--- bfd/config.bfd.orig	2012-09-04 16:14:59.000000000 +0200
+++ bfd/config.bfd	2012-11-17 14:55:35.729616193 +0100
@@ -315,7 +315,7 @@ case "${targ}" in
     ;;
   arm-*-elf | arm-*-freebsd* | arm*-*-linux-* | arm*-*-conix* | \
   arm*-*-uclinux* | arm-*-kfreebsd*-gnu | \
-  arm*-*-eabi* )
+  arm*-*-eabi* | arm-*-riscos*)
     targ_defvec=bfd_elf32_littlearm_vec
     targ_selvecs=bfd_elf32_bigarm_vec
     ;;
