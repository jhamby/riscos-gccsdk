--- bfd/config.bfd.orig	2010-12-01 12:03:10.000000000 +0100
+++ bfd/config.bfd	2010-12-11 02:00:02.753464116 +0100
@@ -281,7 +281,7 @@ case "${targ}" in
     ;;
   arm-*-elf | arm-*-freebsd* | arm*-*-linux-* | arm*-*-conix* | \
   arm*-*-uclinux* | arm-*-kfreebsd*-gnu | \
-  arm*-*-eabi* )
+  arm*-*-eabi* | arm-*-riscos*)
     targ_defvec=bfd_elf32_littlearm_vec
     targ_selvecs=bfd_elf32_bigarm_vec
     ;;
