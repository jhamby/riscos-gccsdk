--- gcc/config.gcc.orig	2006-10-12 00:01:49.000000000 +0200
+++ gcc/config.gcc	2006-10-12 00:03:31.000000000 +0200
@@ -723,6 +723,20 @@
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h arm/rtems-elf.h rtems.h"
 	tmake_file="arm/t-arm arm/t-arm-elf t-rtems arm/t-rtems"
 	;;
+arm*-elf*-riscos* | arm*-riscos*-elf*)		# ARM RISC OS with ELF
+	tm_file="dbxelf.h elfos.h arm/riscos-elf.h arm/elf.h arm/aout.h arm/arm.h"
+	tmake_file="${tmake_file} arm/t-arm arm/t-arm-riscos-elf"
+	extra_options="${extra_options} arm/riscos.opt"
+	extra_objs="riscos.o"
+	extra_gcc_objs="riscos.o"
+	;;
+arm*-aof*-riscos* | arm*-riscos*-aof*)		# ARM RISC OS with AOF
+	tm_file="arm/riscos-aof.h arm/arm.h"
+	tmake_file="${tmake_file} arm/t-arm arm/t-arm-riscos-aof"
+	extra_options="${extra_options} arm/riscos.opt"
+	extra_objs="riscos.o"
+	extra_gcc_objs="riscos.o"
+	;;
 arm*-*-elf | ep9312-*-elf)
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h"
 	tmake_file="arm/t-arm arm/t-arm-elf"
