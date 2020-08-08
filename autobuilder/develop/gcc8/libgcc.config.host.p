--- libgcc/config.host.orig	2018-05-11 20:45:41.436569431 +0100
+++ libgcc/config.host	2018-05-20 10:43:23.532143940 +0100
@@ -235,6 +235,13 @@
   tmake_file="$tmake_file t-crtstuff-pic t-libgcc-pic t-eh-dw2-dip t-slibgcc t-slibgcc-fuchsia"
   extra_parts="crtbegin.o crtend.o"
   ;;
+arm*-riscos-gnueabihf)
+  tmake_file="$tmake_file t-crtstuff-pic t-libgcc-pic t-eh-dw2-dip t-slibgcc t-slibgcc-gld t-slibgcc-elf-ver t-linux"
+  extra_parts="crtbegin.o crtbeginS.o crtbeginT.o crtend.o crtendS.o crti.o crtn.o"
+  if test x$enable_vtable_verify = xyes; then
+    extra_parts="$extra_parts vtv_start.o vtv_end.o vtv_start_preinit.o vtv_end_preinit.o"
+  fi
+  ;;
 *-*-linux* | frv-*-*linux* | *-*-kfreebsd*-gnu | *-*-gnu* | *-*-kopensolaris*-gnu)
   tmake_file="$tmake_file t-crtstuff-pic t-libgcc-pic t-eh-dw2-dip t-slibgcc t-slibgcc-gld t-slibgcc-elf-ver t-linux"
   extra_parts="crtbegin.o crtbeginS.o crtbeginT.o crtend.o crtendS.o"
@@ -421,6 +428,14 @@
 arm*-*-netbsdelf*)
 	tmake_file="$tmake_file arm/t-arm arm/t-netbsd t-slibgcc-gld-nover"
 	;;
+arm*-riscos-gnueabihf)
+	tmake_file="${tmake_file} arm/t-arm t-fixedpoint-gnu-prefix t-crtfm"
+	tmake_file="${tmake_file} arm/t-elf arm/t-bpabi arm/t-linux-eabi t-slibgcc-libgcc t-slibgcc-nolc-override"
+	tm_file="$tm_file arm/bpabi-lib.h"
+	unwind_header=config/arm/unwind-arm.h
+	tmake_file="$tmake_file t-softfp-sfdf t-softfp-excl arm/t-softfp t-softfp"
+	extra_parts="$extra_parts crtfastmath.o"
+	;;
 arm*-*-linux*)			# ARM GNU/Linux with ELF
 	tmake_file="${tmake_file} arm/t-arm t-fixedpoint-gnu-prefix t-crtfm"
 	tmake_file="${tmake_file} arm/t-elf arm/t-bpabi arm/t-linux-eabi t-slibgcc-libgcc"
