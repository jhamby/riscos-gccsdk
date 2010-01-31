--- gcc/config/arm/arm.opt.orig	2008-06-05 01:43:01.000000000 +0200
+++ gcc/config/arm/arm.opt	2008-06-09 23:24:42.000000000 +0200
@@ -43,7 +43,8 @@ Target Report Mask(APCS_REENT)
 Generate re-entrant, PIC code
 
 mapcs-stack-check
-Target Report Mask(APCS_STACK) Undocumented
+Target Report Mask(APCS_STACK) Var(target_apcs_stack_check) Explicit
+Generate code to check the amount of stack space available
 
 march=
 Target RejectNegative Joined
