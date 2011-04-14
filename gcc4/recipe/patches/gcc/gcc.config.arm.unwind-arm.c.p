Index: gcc/config/arm/unwind-arm.c
===================================================================
--- gcc/config/arm/unwind-arm.c	(revision 169815)
+++ gcc/config/arm/unwind-arm.c	(working copy)
@@ -44,6 +44,10 @@
 __gnu_Unwind_Find_exidx (_Unwind_Ptr, int *);
 
 /* Misc constants.  */
+#ifdef __riscos__
+#define R_SL	10
+#define R_FP	11
+#endif
 #define R_IP	12
 #define R_SP	13
 #define R_LR	14
@@ -183,6 +187,32 @@
 void __gnu_Unwind_Save_VFP_D_16_to_31 (struct vfpv3_regs * p);
 void __gnu_Unwind_Restore_VFP_D_16_to_31 (struct vfpv3_regs * p);
 
+#ifdef __riscos__
+
+void __ehs_unwind_stack_chunk (void **fp,
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
@@ -210,9 +240,10 @@
 extern int __text_start;
 extern int __data_start;
 
-/* The exception index table location.  */
-extern __EIT_entry __exidx_start;
-extern __EIT_entry __exidx_end;
+/* The exception index table location.
+   RISC OS: These must be const so that in module code, they are not relocated.  */
+extern const __EIT_entry __exidx_start;
+extern const __EIT_entry __exidx_end;
 
 /* ABI defined personality routines.  */
 extern _Unwind_Reason_Code __aeabi_unwind_cpp_pr0 (_Unwind_State,
@@ -590,7 +621,7 @@
 {
   const __EIT_entry * eitp;
   int nrec;
-  
+
   /* The return address is the address of the instruction following the
      call instruction (plus one in thumb mode).  If this was the last
      instruction in the function the address will lie in the following
@@ -682,6 +713,12 @@
 
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&vrs->core.r[R_FP],
+				(void **)&vrs->core.r[R_PC],
+				(void **)&vrs->core.r[R_SL]);
+#endif
+
       /* Find the entry for this routine.  */
       if (get_eit_entry (ucbp, vrs->core.r[R_PC]) != _URC_OK)
 	abort ();
@@ -696,7 +733,11 @@
   
   if (pr_result != _URC_INSTALL_CONTEXT)
     abort();
-  
+
+#ifdef __riscos__
+  fixup_stack (&vrs->core);
+#endif
+
   restore_core_regs (&vrs->core);
 }
 
@@ -726,6 +767,12 @@
       _Unwind_Reason_Code entry_code;
       _Unwind_Reason_Code stop_code;
 
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&saved_vrs.core.r[R_FP],
+				(void **)&saved_vrs.core.r[R_PC],
+				(void **)&saved_vrs.core.r[R_SL]);
+#endif
+
       /* Find the entry for this routine.  */
       entry_code = get_eit_entry (ucbp, saved_vrs.core.r[R_PC]);
 
@@ -782,6 +829,10 @@
       return _URC_FAILURE;
     }
 
+#ifdef __riscos__
+  fixup_stack (&saved_vrs.core);
+#endif
+
   restore_core_regs (&saved_vrs.core);
 }
 
@@ -818,10 +869,16 @@
   saved_vrs.core = entry_vrs->core;
   /* Set demand-save flags.  */
   saved_vrs.demand_save_flags = ~(_uw) 0;
-  
+
   /* Unwind until we reach a propagation barrier.  */
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&saved_vrs.core.r[R_FP],
+				(void **)&saved_vrs.core.r[R_PC],
+				(void **)&saved_vrs.core.r[R_SL]);
+#endif
+
       /* Find the entry for this routine.  */
       if (get_eit_entry (ucbp, saved_vrs.core.r[R_PC]) != _URC_OK)
 	return _URC_FAILURE;
@@ -892,6 +949,9 @@
   switch (pr_result)
     {
     case _URC_INSTALL_CONTEXT:
+#ifdef __riscos__
+      fixup_stack (&entry_vrs->core);
+#endif
       /* Upload the registers to enter the landing pad.  */
       restore_core_regs (&entry_vrs->core);
 
@@ -970,6 +1030,12 @@
   
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&saved_vrs.core.r[R_FP],
+				(void **)&saved_vrs.core.r[R_PC],
+				(void **)&saved_vrs.core.r[R_SL]);
+#endif
+
       /* Find the entry for this routine.  */
       if (get_eit_entry (ucbp, saved_vrs.core.r[R_PC]) != _URC_OK)
 	{
