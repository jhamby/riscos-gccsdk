Index: libgcc/config/arm/unwind-arm.c
===================================================================
--- libgcc/config/arm/unwind-arm.c	(revision 178826)
+++ libgcc/config/arm/unwind-arm.c	(working copy)
@@ -24,6 +24,10 @@
 #include "unwind.h"
 
 /* Misc constants.  */
+#ifdef __riscos__
+# define R_SL	10
+# define R_FP	11
+#endif
 #define R_IP	12
 #define R_SP	13
 #define R_LR	14
@@ -31,6 +35,10 @@
 
 #define VRS_PC(vrs) ((vrs)->core.r[R_PC])
 #define VRS_SP(vrs) ((vrs)->core.r[R_SP])
+#ifdef __riscos__
+# define VRS_SL(vrs) ((vrs)->core.r[R_SL])
+# define VRS_FP(vrs) ((vrs)->core.r[R_FP])
+#endif
 #define VRS_RETURN(vrs) ((vrs)->core.r[R_LR])
 
 struct core_regs
@@ -130,6 +138,33 @@
 void __gnu_Unwind_Save_VFP_D_16_to_31 (struct vfpv3_regs * p);
 void __gnu_Unwind_Restore_VFP_D_16_to_31 (struct vfpv3_regs * p);
 
+#ifdef __riscos__
+
+void __ehs_unwind_stack_chunk (void **current_fp,
+			       void *alloca_fp,
+			       void **pc,
+			       void **sl);
+void __ehs_trim_stack (void);
+
+/* Make sure that when we return to the exception handler, any
+   excess stack chunks are freed.
+   Note that __ehs_unwind_stack_chunk tracks sl, so we don't have
+   to restore it here.  */
+static void
+fixup_stack (struct core_regs *core)
+{
+  _uw *sp = (_uw *)core->r[R_SP];
+
+  /* Store the address of the landing pad on the stack of the
+     target frame so that __ehs_trim_stack() can retrieve it later.  */
+  sp[-1] = core->r[R_PC];
+
+  /* Make __ehs_trim_stack() the target destination. */
+  core->r[R_PC] = (_uw)__ehs_trim_stack;
+}
+
+#endif
+
 /* Restore coprocessor state after phase1 unwinding.  */
 static void
 restore_non_core_regs (phase1_vrs * vrs)
