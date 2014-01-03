--- bfd/config.bfd.orig	2013-11-04 16:33:37.000000000 +0100
+++ bfd/config.bfd	2013-12-31 13:16:36.386127574 +0100
@@ -333,7 +333,7 @@ case "${targ}" in
     ;;
   arm-*-elf | arm-*-freebsd* | arm*-*-linux-* | arm*-*-conix* | \
   arm*-*-uclinux* | arm-*-kfreebsd*-gnu | \
-  arm*-*-eabi* )
+  arm*-*-eabi* | arm-*-riscos*)
     targ_defvec=bfd_elf32_littlearm_vec
     targ_selvecs=bfd_elf32_bigarm_vec
     ;;
