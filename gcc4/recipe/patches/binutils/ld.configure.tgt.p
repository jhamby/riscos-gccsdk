--- ld/configure.tgt.orig	2013-11-26 12:37:33.000000000 +0100
+++ ld/configure.tgt	2013-12-31 13:16:36.482126367 +0100
@@ -94,6 +94,7 @@ arm-*-nto*)		targ_emul=armnto ;;
 arm-*-openbsd*)		targ_emul=armnbsd ;;
 arm-*-rtems*)		targ_emul=armelf ;;
 armeb-*-elf)		targ_emul=armelfb ;;
+arm*-*-riscos*)		targ_emul=armelf_riscos ;;
 arm-*-elf | arm*-*-eabi*)
 	  		targ_emul=armelf ;;
 arm*-*-symbianelf*)     targ_emul=armsymbian;;
