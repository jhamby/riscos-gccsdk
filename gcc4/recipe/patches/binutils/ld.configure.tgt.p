--- ld/configure.tgt.orig	2011-11-21 10:29:37.000000000 +0100
+++ ld/configure.tgt	2011-11-21 19:52:51.426282193 +0100
@@ -66,6 +66,7 @@ arm-*-nto*)		targ_emul=armnto ;;
 arm-*-openbsd*)		targ_emul=armnbsd ;;
 arm-*-rtems*)		targ_emul=armelf ;;
 armeb-*-elf)		targ_emul=armelfb ;;
+arm*-*-riscos*)		targ_emul=armelf_riscos ;;
 arm-*-elf | arm*-*-eabi*)
 	  		targ_emul=armelf ;;
 arm*-*-symbianelf*)     targ_emul=armsymbian;;
