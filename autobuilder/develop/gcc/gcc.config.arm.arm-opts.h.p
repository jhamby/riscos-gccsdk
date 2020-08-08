--- gcc/config/arm/arm-opts.h.orig	2020-07-23 07:35:17.340384509 +0100
+++ gcc/config/arm/arm-opts.h	2020-07-24 17:16:30.625916279 +0100
@@ -47,7 +47,8 @@
   ARM_ABI_ATPCS,
   ARM_ABI_AAPCS,
   ARM_ABI_IWMMXT,
-  ARM_ABI_AAPCS_LINUX
+  ARM_ABI_AAPCS_LINUX,
+  ARM_ABI_APCS32
 };
 
 enum float_abi_type
