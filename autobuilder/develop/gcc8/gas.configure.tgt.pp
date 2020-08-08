--- gas/configure.tgt.orig	2018-01-13 13:31:15.000000000 +0000
+++ gas/configure.tgt	2018-05-25 19:28:05.108240632 +0100
@@ -144,6 +144,7 @@
   arm-*-coff)				fmt=coff ;;
   arm-*-phoenix*)			fmt=elf ;;
   arm-*-elf)				fmt=elf ;;
+  arm-riscos-gnueabihf)			fmt=elf em=riscos ;;
   arm-*-eabi* | arm-*-rtems*)		fmt=elf em=armeabi ;;
   arm-*-symbianelf*)			fmt=elf em=symbian ;;
   arm-*-kaos*)				fmt=elf ;;
