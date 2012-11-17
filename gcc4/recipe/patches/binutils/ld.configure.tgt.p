--- ld/configure.tgt.orig	2012-09-04 14:53:47.000000000 +0200
+++ ld/configure.tgt	2012-11-17 14:55:35.945613478 +0100
@@ -74,6 +74,7 @@ arm-*-nto*)		targ_emul=armnto ;;
 arm-*-openbsd*)		targ_emul=armnbsd ;;
 arm-*-rtems*)		targ_emul=armelf ;;
 armeb-*-elf)		targ_emul=armelfb ;;
+arm*-*-riscos*)		targ_emul=armelf_riscos ;;
 arm-*-elf | arm*-*-eabi*)
 	  		targ_emul=armelf ;;
 arm*-*-symbianelf*)     targ_emul=armsymbian;;
