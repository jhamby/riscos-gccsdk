--- gcc/config.gcc.orig	2007-12-21 00:05:58.000000000 +0100
+++ gcc/config.gcc	2007-12-20 21:23:26.000000000 +0100
@@ -723,6 +723,13 @@
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h arm/rtems-elf.h rtems.h"
 	tmake_file="arm/t-arm arm/t-arm-elf t-rtems arm/t-rtems"
 	;;
+arm*-*-riscos)	# ARM RISC OS with ELF
+	tm_file="dbxelf.h elfos.h arm/riscos-elf.h arm/elf.h arm/aout.h arm/arm.h"
+	tmake_file="${tmake_file} arm/t-arm arm/t-arm-riscos-elf"
+	extra_options="${extra_options} arm/riscos.opt"
+	extra_objs="riscos.o"
+	extra_gcc_objs="riscos-gcc.o"
+	;;
 arm*-*-elf | ep9312-*-elf)
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h"
 	tmake_file="arm/t-arm arm/t-arm-elf"
