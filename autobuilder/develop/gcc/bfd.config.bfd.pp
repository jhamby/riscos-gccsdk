--- bfd/config.bfd.orig	2018-01-13 13:31:15.000000000 +0000
+++ bfd/config.bfd	2018-06-08 11:04:37.001777520 +0100
@@ -442,6 +442,11 @@
     targ_defvec=arm_elf32_le_vec
     targ_selvecs=arm_elf32_be_vec
     ;;
+  arm-riscos-gnueabihf)
+    targ_defvec=arm_elf32_le_vec
+    targ_selvecs=arm_elf32_be_vec
+    targ_cflags=-D__RISCOS_TARGET__
+    ;;
   arm-*-elf | arm*-*-freebsd* | arm*-*-linux-* | arm*-*-conix* | \
   arm*-*-uclinux* | arm-*-kfreebsd*-gnu | \
   arm*-*-eabi* | arm-*-rtems*)
