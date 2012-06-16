Index: gcc/config/arm/arm.opt
===================================================================
--- gcc/config/arm/arm.opt	(revision 175348)
+++ gcc/config/arm/arm.opt	(working copy)
@@ -52,6 +52,9 @@
 Enum(arm_abi_type) String(iwmmxt) Value(ARM_ABI_IWMMXT)
 
 EnumValue
+Enum(arm_abi_type) String(apcs-32) Value(ARM_ABI_APCS32)
+
+EnumValue
 Enum(arm_abi_type) String(aapcs-linux) Value(ARM_ABI_AAPCS_LINUX)
 
 mabort-on-noreturn
