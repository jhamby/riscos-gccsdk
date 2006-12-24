--- gcc/config/arm/elf.h.orig	2006-12-23 22:21:57.000000000 -0800
+++ gcc/config/arm/elf.h	2006-12-23 21:00:44.000000000 -0800
@@ -46,7 +46,8 @@
 
 #ifndef SUBTARGET_ASM_FLOAT_SPEC
 #define SUBTARGET_ASM_FLOAT_SPEC "\
-%{mapcs-float:-mfloat}"
+%{mapcs-float:-mfloat} \
+%{!mhard-float:-mfloat-abi=soft}"
 #endif
 
 #ifndef ASM_SPEC
