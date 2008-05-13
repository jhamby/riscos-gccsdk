--- gcc/config.gcc.orig	2008-05-11 18:20:10.000000000 +0200
+++ gcc/config.gcc	2008-05-11 18:20:02.000000000 +0200
@@ -723,6 +723,14 @@
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h arm/rtems-elf.h rtems.h"
 	tmake_file="arm/t-arm arm/t-arm-elf t-rtems arm/t-rtems"
 	;;
+arm*-*-riscos)	# ARM RISC OS with ELF
+	tm_file="dbxelf.h elfos.h arm/riscos-elf.h arm/elf.h arm/aout.h arm/arm.h"
+	tmake_file="${tmake_file} arm/t-arm arm/t-arm-riscos-elf"
+	xm_file="arm/xm-riscos.h"
+	extra_options="${extra_options} arm/riscos.opt"
+	extra_objs="riscos.o"
+	extra_gcc_objs="riscos-gcc.o"
+	;;
 arm*-*-elf | ep9312-*-elf)
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h"
 	tmake_file="arm/t-arm arm/t-arm-elf"
