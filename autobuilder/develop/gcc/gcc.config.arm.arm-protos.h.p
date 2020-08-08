--- gcc/config/arm/arm-protos.h.orig	2020-07-24 17:16:30.845914454 +0100
+++ gcc/config/arm/arm-protos.h	2020-07-24 21:14:56.937469244 +0100
@@ -583,4 +583,14 @@
 const char * arm_gen_far_branch (rtx *, int, const char * , const char *);
 
 bool arm_mve_immediate_check(rtx, machine_mode, bool);
+
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
