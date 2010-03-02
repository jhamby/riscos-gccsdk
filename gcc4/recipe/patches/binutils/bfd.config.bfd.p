--- bfd/config.bfd.orig	2009-08-06 19:38:00.000000000 +0200
+++ bfd/config.bfd	2010-01-31 16:19:42.035012118 +0100
@@ -282,7 +282,7 @@ case "${targ}" in
     ;;
   arm-*-elf | arm-*-freebsd* | arm*-*-linux-* | arm*-*-conix* | \
   arm*-*-uclinux* | arm-*-kfreebsd*-gnu | \
-  arm*-*-eabi* )
+  arm*-*-eabi* | arm-*-riscos*)
     targ_defvec=bfd_elf32_littlearm_vec
     targ_selvecs=bfd_elf32_bigarm_vec
     ;;
