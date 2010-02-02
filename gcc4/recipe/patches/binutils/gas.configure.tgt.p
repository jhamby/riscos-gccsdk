--- gas/configure.tgt.orig	2009-09-09 10:13:28.000000000 +0200
+++ gas/configure.tgt	2010-01-31 16:19:42.175009843 +0100
@@ -126,6 +126,7 @@ case ${generic_target} in
 					fmt=coff em=wince-pe ;;
   arm-*-pe | thumb-*-pe)		fmt=coff em=pe ;;
   arm-*-riscix*)			fmt=aout em=riscix ;;
+  arm-*-riscos*)			fmt=elf em=riscos ;;
 
   avr-*-*)				fmt=elf bfd_gas=yes ;;
 
