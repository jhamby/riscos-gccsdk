Index: gcc/config/arm/unwind-arm.c
===================================================================
--- gcc/config/arm/unwind-arm.c	(revision 172858)
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
@@ -183,6 +187,33 @@
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
@@ -210,9 +241,10 @@
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
@@ -590,7 +622,7 @@
 {
   const __EIT_entry * eitp;
   int nrec;
-  
+
   /* The return address is the address of the instruction following the
      call instruction (plus one in thumb mode).  If this was the last
      instruction in the function the address will lie in the following
@@ -682,6 +714,14 @@
 
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&vrs->core.r[R_FP],
+				(void *)ucbp->alloca_fp,
+				(void **)&vrs->core.r[R_PC],
+				(void **)&vrs->core.r[R_SL]);
+      ucbp->alloca_fp = vrs->core.r[R_FP];
+#endif
+
       /* Find the entry for this routine.  */
       if (get_eit_entry (ucbp, vrs->core.r[R_PC]) != _URC_OK)
 	abort ();
@@ -696,7 +736,11 @@
   
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
 
@@ -726,6 +770,14 @@
       _Unwind_Reason_Code entry_code;
       _Unwind_Reason_Code stop_code;
 
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&saved_vrs.core.r[R_FP],
+				(void *)ucbp->alloca_fp,
+				(void **)&saved_vrs.core.r[R_PC],
+				(void **)&saved_vrs.core.r[R_SL]);
+      ucbp->alloca_fp = saved_vrs.core.r[R_FP];
+#endif
+
       /* Find the entry for this routine.  */
       entry_code = get_eit_entry (ucbp, saved_vrs.core.r[R_PC]);
 
@@ -782,6 +834,10 @@
       return _URC_FAILURE;
     }
 
+#ifdef __riscos__
+  fixup_stack (&saved_vrs.core);
+#endif
+
   restore_core_regs (&saved_vrs.core);
 }
 
@@ -811,6 +867,10 @@
   phase1_vrs saved_vrs;
   _Unwind_Reason_Code pr_result;
 
+#ifdef __riscos__
+  ucbp->alloca_fp = entry_vrs->core.r[R_FP];
+#endif
+
   /* Set the pc to the call site.  */
   entry_vrs->core.r[R_PC] = entry_vrs->core.r[R_LR];
 
@@ -818,10 +878,18 @@
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
+				(void *)ucbp->alloca_fp,
+				(void **)&saved_vrs.core.r[R_PC],
+				(void **)&saved_vrs.core.r[R_SL]);
+      ucbp->alloca_fp = saved_vrs.core.r[R_FP];
+#endif
+
       /* Find the entry for this routine.  */
       if (get_eit_entry (ucbp, saved_vrs.core.r[R_PC]) != _URC_OK)
 	return _URC_FAILURE;
@@ -841,7 +909,7 @@
 	 pr returned _URC_FAILURE.  */
       return _URC_FAILURE;
     }
-  
+
   unwind_phase2 (ucbp, entry_vrs);
 }
 
@@ -885,6 +953,10 @@
       abort ();
     }
 
+#ifdef __riscos__
+  ucbp->alloca_fp = entry_vrs->core.r[R_FP];
+#endif
+
   /* Call the cached PR.  */
   pr_result = ((personality_routine) UCB_PR_ADDR (ucbp))
 	(_US_UNWIND_FRAME_RESUME, ucbp, (_Unwind_Context *) entry_vrs);
@@ -892,6 +964,9 @@
   switch (pr_result)
     {
     case _URC_INSTALL_CONTEXT:
+#ifdef __riscos__
+      fixup_stack (&entry_vrs->core);
+#endif
       /* Upload the registers to enter the landing pad.  */
       restore_core_regs (&entry_vrs->core);
 
@@ -960,6 +1035,10 @@
   _Unwind_Control_Block ucb;
   _Unwind_Control_Block *ucbp = &ucb;
 
+#ifdef __riscos__
+  ucbp->alloca_fp = entry_vrs->core.r[R_FP];
+#endif
+
   /* Set the pc to the call site.  */
   entry_vrs->core.r[R_PC] = entry_vrs->core.r[R_LR];
 
@@ -970,6 +1049,14 @@
   
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&saved_vrs.core.r[R_FP],
+				(void *)ucbp->alloca_fp,
+				(void **)&saved_vrs.core.r[R_PC],
+				(void **)&saved_vrs.core.r[R_SL]);
+      ucbp->alloca_fp = saved_vrs.core.r[R_FP];
+#endif
+
       /* Find the entry for this routine.  */
       if (get_eit_entry (ucbp, saved_vrs.core.r[R_PC]) != _URC_OK)
 	{
