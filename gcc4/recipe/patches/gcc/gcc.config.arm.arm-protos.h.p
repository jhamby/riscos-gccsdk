Index: gcc/config/arm/arm-protos.h
===================================================================
--- gcc/config/arm/arm-protos.h	(revision 183074)
+++ gcc/config/arm/arm-protos.h	(working copy)
@@ -247,4 +247,13 @@
 extern void arm_expand_vec_perm (rtx target, rtx op0, rtx op1, rtx sel);
 extern bool arm_expand_vec_perm_const (rtx target, rtx op0, rtx op1, rtx sel);
 
+extern bool arm_apcs_frame_needed (void);
+extern void arm_expand_save_stack_block (rtx, rtx);
+extern void arm_expand_restore_stack_block (rtx, rtx);
+extern void arm_expand_alloca_epilogue (void);
+extern void arm_expand_allocate_stack (rtx, rtx);
+extern void arm_expand_save_stack_nonlocal (rtx *);
+extern void arm_expand_restore_stack_nonlocal (rtx *);
+extern void arm_expand_nonlocal_goto (rtx *);
+
 #endif /* ! GCC_ARM_PROTOS_H */
