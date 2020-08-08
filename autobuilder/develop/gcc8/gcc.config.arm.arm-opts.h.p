Index: gcc/config/arm/arm-opts.h
===================================================================
--- gcc/config/arm/arm-opts.h	(revision 220899)
+++ gcc/config/arm/arm-opts.h	(working copy)
@@ -55,7 +55,8 @@
   ARM_ABI_ATPCS,
   ARM_ABI_AAPCS,
   ARM_ABI_IWMMXT,
-  ARM_ABI_AAPCS_LINUX
+  ARM_ABI_AAPCS_LINUX,
+  ARM_ABI_APCS32
 };
 
 enum float_abi_type
