--- gas/configure.tgt.orig	2013-11-04 16:33:37.000000000 +0100
+++ gas/configure.tgt	2013-12-31 13:16:36.478126417 +0100
@@ -150,6 +150,7 @@ case ${generic_target} in
 					fmt=coff em=wince-pe ;;
   arm-*-pe)				fmt=coff em=pe ;;
   arm-*-riscix*)			fmt=aout em=riscix ;;
+  arm-*-riscos*)			fmt=elf em=riscos ;;
 
   avr-*-*)				fmt=elf bfd_gas=yes ;;
 
