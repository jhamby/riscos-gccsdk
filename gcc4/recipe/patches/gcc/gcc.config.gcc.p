Index: gcc/config.gcc
===================================================================
--- gcc/config.gcc	(revision 196237)
+++ gcc/config.gcc	(working copy)
@@ -930,6 +930,18 @@
 	tm_file="dbxelf.h elfos.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h arm/rtems-elf.h rtems.h newlib-stdint.h"
 	tmake_file="arm/t-arm arm/t-arm-elf t-rtems arm/t-rtems"
 	;;
+arm*-*-riscos)
+	tm_file="dbxelf.h elfos.h arm/riscos-elf.h arm/elf.h arm/aout.h arm/arm.h"
+	# FIXME: when --enable-shared option is given, we shouldn't have t-slibgcc
+	# specified.  It's only there to let SHLIB and ENABLE_SHARED_LIBGCC
+	# being defined so REAL_LIBGCC_SPEC is correctly defined for the shared vs
+	# static cases.
+	tmake_file="arm/t-arm arm/t-arm-riscos-elf t-slibgcc"
+	xm_file="arm/xm-riscos.h"
+	extra_options="${extra_options} arm/riscos.opt"
+	extra_objs="riscos.o"
+	extra_gcc_objs="riscos-gcc.o"
+	;;
 arm*-*-elf)
 	tm_file="dbxelf.h elfos.h newlib-stdint.h arm/unknown-elf.h arm/elf.h arm/aout.h arm/arm.h"
 	tmake_file="arm/t-arm arm/t-arm-elf"
