--- ld/configure.tgt.orig	2010-12-01 12:03:47.000000000 +0100
+++ ld/configure.tgt	2010-12-11 02:00:02.872501343 +0100
@@ -87,6 +87,7 @@ arm*-*-linux-*)	        targ_emul=armelf
 			targ_extra_emuls="armelf armelfb armelfb_linux"
 			targ_extra_libpath="armelfb_linux"
 			;;
+arm*-*-riscos*)		targ_emul=armelf_riscos ;;
 arm*-*-uclinux*eabi)	targ_emul=armelf_linux_eabi
 			targ_extra_emuls=armelfb_linux_eabi
 			targ_extra_libpath=$targ_extra_emuls
