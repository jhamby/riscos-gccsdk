--- ld/configure.tgt.orig	2018-01-13 13:31:16.000000000 +0000
+++ ld/configure.tgt	2018-05-25 17:15:45.012137740 +0100
@@ -123,6 +123,7 @@
 arm-*-phoenix*)		targ_emul=armelf ;;
 armeb-*-elf | armeb-*-eabi*)
 			targ_emul=armelfb ;;
+arm-riscos-gnueabihf)	targ_emul=armelf_riscos_eabi ;;
 arm-*-elf | arm*-*-eabi* | arm-*-rtems*)
 			targ_emul=armelf ;;
 arm*-*-symbianelf*)	targ_emul=armsymbian;;
