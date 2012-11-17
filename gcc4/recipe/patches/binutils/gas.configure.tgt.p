--- gas/configure.tgt.orig	2012-09-04 14:53:45.000000000 +0200
+++ gas/configure.tgt	2012-11-17 14:55:35.909613930 +0100
@@ -132,6 +132,7 @@ case ${generic_target} in
 					fmt=coff em=wince-pe ;;
   arm-*-pe)				fmt=coff em=pe ;;
   arm-*-riscix*)			fmt=aout em=riscix ;;
+  arm-*-riscos*)			fmt=elf em=riscos ;;
 
   avr-*-*)				fmt=elf bfd_gas=yes ;;
 
