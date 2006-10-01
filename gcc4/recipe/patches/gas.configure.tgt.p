--- gas/configure.tgt.orig	2006-09-25 03:15:51.000000000 +0200
+++ gas/configure.tgt	2006-09-25 03:15:52.000000000 +0200
@@ -117,6 +117,7 @@
   arm-wince-pe | arm-*-wince)		fmt=coff em=wince-pe ;;
   arm-*-pe | thumb-*-pe)		fmt=coff em=pe ;;
   arm-*-riscix*)			fmt=aout em=riscix ;;
+  arm-*-riscos*)			fmt=elf em=riscos ;;
 
   avr-*-*)				fmt=elf bfd_gas=yes ;;
   bfin-*-*)				fmt=elf bfd_gas=yes ;;
