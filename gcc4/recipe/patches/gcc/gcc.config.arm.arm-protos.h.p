--- gcc/config/arm/arm-protos.h.orig	2007-12-21 00:05:01.000000000 +0100
+++ gcc/config/arm/arm-protos.h	2007-12-20 20:15:27.000000000 +0100
@@ -124,9 +124,13 @@
 extern const char *vfp_output_fstmx (rtx *);
 extern void arm_set_return_address (rtx, rtx);
 extern int arm_eliminable_register (rtx);
+extern int arm_can_eliminate (int, int);
+extern bool arm_apcs_frame_needed (void);
 
 extern bool arm_output_addr_const_extra (FILE *, rtx);
 
+extern int arm_function_arg_regno_p (int regno);
+
 #if defined TREE_CODE
 extern rtx arm_function_arg (CUMULATIVE_ARGS *, enum machine_mode, tree, int);
 extern void arm_init_cumulative_args (CUMULATIVE_ARGS *, tree, rtx, tree);
@@ -190,4 +194,12 @@
 extern void arm_pr_no_long_calls (struct cpp_reader *);
 extern void arm_pr_long_calls_off (struct cpp_reader *);
 
+extern void arm_expand_save_stack_block (rtx, rtx);
+extern void arm_expand_restore_stack_block (rtx, rtx);
+extern void arm_expand_alloca_epilogue (void);
+extern void arm_expand_allocate_stack (rtx, rtx);
+extern void arm_expand_save_stack_nonlocal (rtx *);
+extern void arm_expand_restore_stack_nonlocal (rtx *);
+extern void arm_expand_nonlocal_goto (rtx *);
+
 #endif /* ! GCC_ARM_PROTOS_H */
