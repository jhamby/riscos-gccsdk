--- gcc/config.gcc.orig	2006-11-14 00:30:44.000000000 +0100
+++ gcc/config.gcc	2006-11-14 00:30:30.000000000 +0100
@@ -723,6 +723,13 @@
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h arm/rtems-elf.h rtems.h"
 	tmake_file="arm/t-arm arm/t-arm-elf t-rtems arm/t-rtems"
 	;;
+arm*-*-riscoself* | arm*-*-riscos | arm*-riscos*-elf*)	# ARM RISC OS with ELF
+	tm_file="dbxelf.h elfos.h arm/riscos-elf.h arm/elf.h arm/aout.h arm/arm.h"
+	tmake_file="${tmake_file} arm/t-arm arm/t-arm-riscos-elf"
+	extra_options="${extra_options} arm/riscos.opt"
+	extra_objs="riscos.o"
+	extra_gcc_objs="riscos.o"
+	;;
 arm*-*-elf | ep9312-*-elf)
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h"
 	tmake_file="arm/t-arm arm/t-arm-elf"
