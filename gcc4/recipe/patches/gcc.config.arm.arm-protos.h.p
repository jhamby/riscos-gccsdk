--- gcc/config/arm/arm-protos.h.orig	2006-09-25 01:20:59.000000000 +0200
+++ gcc/config/arm/arm-protos.h	2006-09-25 01:32:53.000000000 +0200
@@ -124,6 +124,8 @@
 extern const char *vfp_output_fstmx (rtx *);
 extern void arm_set_return_address (rtx, rtx);
 extern int arm_eliminable_register (rtx);
+extern int arm_can_eliminate (int, int);
+extern bool arm_apcs_frame_needed (void);
 
 extern bool arm_output_addr_const_extra (FILE *, rtx);
 
@@ -190,4 +192,12 @@
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
