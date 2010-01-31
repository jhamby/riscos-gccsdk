--- bfd/config.bfd.orig	2006-09-25 03:09:45.000000000 +0200
+++ bfd/config.bfd	2006-09-25 03:09:46.000000000 +0200
@@ -252,7 +252,7 @@
     ;;
   arm-*-elf | arm-*-freebsd* | arm*-*-linux-* | arm*-*-conix* | \
   arm*-*-uclinux* | arm-*-kfreebsd*-gnu | \
-  arm*-*-eabi* )
+  arm*-*-eabi* | arm-*-riscos* | arm-riscos*)
     targ_defvec=bfd_elf32_littlearm_vec
     targ_selvecs=bfd_elf32_bigarm_vec
     ;;
