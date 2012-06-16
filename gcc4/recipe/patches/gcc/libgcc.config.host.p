Index: libgcc/config.host
===================================================================
--- libgcc/config.host	(revision 183850)
+++ libgcc/config.host	(working copy)
@@ -371,6 +371,13 @@
 	tmake_file="$tmake_file t-softfp-sfdf t-softfp-excl arm/t-softfp t-softfp"
 	unwind_header=config/arm/unwind-arm.h
 	;;
+arm*-*-riscos)
+	tmake_file="${tmake_file} arm/t-arm arm/t-elf t-fixedpoint-gnu-prefix arm/t-arm-riscos-elf"
+	extra_parts="crtbegin.o crtend.o crti.o crtn.o"
+	tmake_file="${tmake_file} t-crtstuff-pic"
+	tmake_file="$tmake_file t-softfp-sfdf t-softfp-excl arm/t-softfp t-softfp"
+	unwind_header=config/arm/unwind-arm.h
+	;;
 arm*-*-rtems*)
 	tmake_file="$tmake_file arm/t-arm arm/t-elf t-softfp-sfdf t-softfp-excl arm/t-softfp t-softfp"
 	extra_parts="$extra_parts crti.o crtn.o"
