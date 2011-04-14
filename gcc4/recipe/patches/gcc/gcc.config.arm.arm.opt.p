Index: gcc/config/arm/arm.opt
===================================================================
--- gcc/config/arm/arm.opt	(revision 161061)
+++ gcc/config/arm/arm.opt	(working copy)
@@ -42,7 +42,8 @@
 Generate re-entrant, PIC code
 
 mapcs-stack-check
-Target Report Mask(APCS_STACK) Undocumented
+Target Report Mask(APCS_STACK) Var(target_apcs_stack_check) Explicit
+Generate code to check the amount of stack space available
 
 march=
 Target RejectNegative Joined
