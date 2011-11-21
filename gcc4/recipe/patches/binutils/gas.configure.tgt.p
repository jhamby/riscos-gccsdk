--- gas/configure.tgt.orig	2011-06-20 15:23:21.000000000 +0200
+++ gas/configure.tgt	2011-11-21 19:42:18.666237063 +0100
@@ -121,6 +121,7 @@ case ${generic_target} in
 					fmt=coff em=wince-pe ;;
   arm-*-pe)				fmt=coff em=pe ;;
   arm-*-riscix*)			fmt=aout em=riscix ;;
+  arm-*-riscos*)			fmt=elf em=riscos ;;
 
   avr-*-*)				fmt=elf bfd_gas=yes ;;
 
