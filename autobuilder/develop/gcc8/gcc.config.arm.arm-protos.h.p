--- gcc/config/arm/arm-protos.h.orig	2016-02-10 12:36:03.000000000 +0000
+++ gcc/config/arm/arm-protos.h	2016-10-25 17:44:24.389363490 +0100
@@ -617,4 +617,13 @@
 
 
 
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
