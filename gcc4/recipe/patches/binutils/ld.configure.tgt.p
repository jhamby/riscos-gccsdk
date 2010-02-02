--- ld/configure.tgt.orig	2009-08-06 19:38:03.000000000 +0200
+++ ld/configure.tgt	2010-01-31 16:59:25.552640063 +0100
@@ -86,6 +86,7 @@ arm*-*-linux-*)	        targ_emul=armelf
 			targ_extra_emuls="armelf armelfb armelfb_linux"
 			targ_extra_libpath="armelfb_linux"
 			;;
+arm*-*-riscos*)		targ_emul=armelf_riscos ;;
 arm*-*-uclinux*eabi)	targ_emul=armelf_linux_eabi
 			targ_extra_emuls=armelfb_linux_eabi
 			targ_extra_libpath=$targ_extra_emuls
