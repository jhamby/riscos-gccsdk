--- ld/configure.tgt.orig	2006-09-25 03:09:46.000000000 +0200
+++ ld/configure.tgt	2006-09-25 03:15:04.000000000 +0200
@@ -63,6 +63,7 @@
 arm*b-*-linux-*eabi)	targ_emul=armelfb_linux_eabi ;;
 arm*b-*-linux-*)	targ_emul=armelfb_linux; targ_extra_emuls=armelfb ;;
 arm*-*-linux-*eabi)	targ_emul=armelf_linux_eabi ;;
+arm*-*-riscos*)		targ_emul=armelf_riscos; targ_extra_emuls="armelf_linux armelf" ;;
 arm*-*-linux-*)	        targ_emul=armelf_linux; targ_extra_emuls=armelf ;;
 arm*-*-uclinux*)	targ_emul=armelf_linux; targ_extra_emuls=armelf ;;
 arm-*-vxworks)		targ_emul=armelf_vxworks ;;
