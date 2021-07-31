Index: libgcc/unwind-arm-common.inc
===================================================================
--- libgcc/unwind-arm-common.inc	(revision 214062)
+++ libgcc/unwind-arm-common.inc	(working copy)
@@ -87,6 +87,33 @@
 
 /* Assembly helper functions.  */
 
+#ifdef __riscos__
+
+static inline unsigned int
+clear_status_bits (unsigned int pc_in)
+{
+  unsigned int pc_out = pc_in;
+
+  asm volatile ("	TEQ	r0, r0\n"
+		"	TEQ	pc, pc\n"
+		"	BICNE	%[pc_out], %[pc_in], #0xfc000003\n"
+		: [pc_out] "=r" (pc_out)
+		: [pc_in] "r" (pc_in)
+		: "cc");
+
+  return pc_out;
+}
+
+#define VRS_PC_NO_PSR(vrs) clear_status_bits((vrs)->core.r[R_PC])
+#define VRS_RETURN_NO_PSR(vrs) clear_status_bits((vrs)->core.r[R_LR])
+
+#else
+
+#define VRS_PC_NO_PSR(vrs) ((vrs)->core.r[R_PC])
+#define VRS_RETURN_NO_PSR(vrs) ((vrs)->core.r[R_LR])
+
+#endif
+
 /* Restore core register state.  Never returns.  */
 void __attribute__((noreturn)) restore_core_regs (struct core_regs *);
 
@@ -101,8 +128,9 @@
 extern int __data_start;
 
 /* The exception index table location.  */
-extern __EIT_entry __exidx_start;
-extern __EIT_entry __exidx_end;
+/* RISC OS: These must be const so that in module code, they are not relocated.  */
+extern const __EIT_entry __exidx_start;
+extern const __EIT_entry __exidx_end;
 
 /* Core unwinding functions.  */
 
@@ -144,7 +172,7 @@
 #define uw_restore_core_regs(TARGET, CORE)				      \
   do									      \
     {									      \
-      void *handler = __builtin_frob_return_addr ((void *) VRS_PC (TARGET));  \
+      void *handler = __builtin_frob_return_addr ((void *) VRS_PC_NO_PSR (TARGET));  \
       _Unwind_DebugHook (0, handler);					      \
       restore_core_regs (CORE);						      \
     }									      \
@@ -284,11 +312,19 @@
 
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&VRS_FP (vrs),
+				(void *)ucbp->alloca_fp,
+				(void **)&VRS_PC (vrs),
+				(void **)&VRS_SL (vrs));
+      ucbp->alloca_fp = VRS_FP (vrs);
+#endif
+
       /* Find the entry for this routine.  */
-      if (get_eit_entry (ucbp, VRS_PC(vrs)) != _URC_OK)
+      if (get_eit_entry (ucbp, VRS_PC_NO_PSR(vrs)) != _URC_OK)
 	abort ();
 
-      UCB_SAVED_CALLSITE_ADDR (ucbp) = VRS_PC(vrs);
+      UCB_SAVED_CALLSITE_ADDR (ucbp) = VRS_PC_NO_PSR(vrs);
 
       /* Call the pr to decide what to do.  */
       pr_result = ((personality_routine) UCB_PR_ADDR (ucbp))
@@ -299,6 +335,10 @@
   if (pr_result != _URC_INSTALL_CONTEXT)
     abort();
 
+#ifdef __riscos__
+  fixup_stack (&vrs->core);
+#endif
+
   uw_restore_core_regs (vrs, &vrs->core);
 }
 
@@ -328,8 +368,16 @@
       _Unwind_Reason_Code entry_code;
       _Unwind_Reason_Code stop_code;
 
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&VRS_FP (&saved_vrs),
+				(void *)ucbp->alloca_fp,
+				(void **)&VRS_PC (&saved_vrs),
+				(void **)&VRS_SL (&saved_vrs));
+      ucbp->alloca_fp = VRS_FP (&saved_vrs);
+#endif
+
       /* Find the entry for this routine.  */
-      entry_code = get_eit_entry (ucbp, VRS_PC (&saved_vrs));
+      entry_code = get_eit_entry (ucbp, VRS_PC_NO_PSR (&saved_vrs));
 
       if (resuming)
 	{
@@ -341,7 +389,7 @@
 
       if (entry_code == _URC_OK)
 	{
-	  UCB_SAVED_CALLSITE_ADDR (ucbp) = VRS_PC (&saved_vrs);
+	  UCB_SAVED_CALLSITE_ADDR (ucbp) = VRS_PC_NO_PSR (&saved_vrs);
 
 	  next_vrs = saved_vrs;
 
@@ -384,6 +432,10 @@
       return _URC_FAILURE;
     }
 
+#ifdef __riscos__
+  fixup_stack (&saved_vrs.core);
+#endif
+
   uw_restore_core_regs (&saved_vrs, &saved_vrs.core);
 }
 
@@ -413,8 +465,12 @@
   phase1_vrs saved_vrs;
   _Unwind_Reason_Code pr_result;
 
+#ifdef __riscos__
+  ucbp->alloca_fp = VRS_FP (entry_vrs);
+#endif
+
   /* Set the pc to the call site.  */
-  VRS_PC (entry_vrs) = VRS_RETURN(entry_vrs);
+  VRS_PC (entry_vrs) = VRS_RETURN_NO_PSR(entry_vrs);
 
   /* Save the core registers.  */
   saved_vrs.core = entry_vrs->core;
@@ -424,8 +480,16 @@
   /* Unwind until we reach a propagation barrier.  */
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&VRS_FP (&saved_vrs),
+				(void *)ucbp->alloca_fp,
+				(void **)&VRS_PC (&saved_vrs),
+				(void **)&VRS_SL (&saved_vrs));
+      ucbp->alloca_fp = VRS_FP (&saved_vrs);
+#endif
+
       /* Find the entry for this routine.  */
-      if (get_eit_entry (ucbp, VRS_PC (&saved_vrs)) != _URC_OK)
+      if (get_eit_entry (ucbp, VRS_PC_NO_PSR (&saved_vrs)) != _URC_OK)
 	return _URC_FAILURE;
 
       /* Call the pr to decide what to do.  */
@@ -463,7 +527,7 @@
   UCB_FORCED_STOP_ARG (ucbp) = (_uw) stop_arg;
 
   /* Set the pc to the call site.  */
-  VRS_PC (entry_vrs) = VRS_RETURN(entry_vrs);
+  VRS_PC (entry_vrs) = VRS_RETURN_NO_PSR(entry_vrs);
 
   return unwind_phase2_forced (ucbp, entry_vrs, 0);
 }
@@ -487,6 +551,10 @@
       abort ();
     }
 
+#ifdef __riscos__
+  ucbp->alloca_fp = VRS_FP (entry_vrs);
+#endif
+
   /* Call the cached PR.  */
   pr_result = ((personality_routine) UCB_PR_ADDR (ucbp))
 	(_US_UNWIND_FRAME_RESUME, ucbp, (_Unwind_Context *) entry_vrs);
@@ -494,6 +562,9 @@
   switch (pr_result)
     {
     case _URC_INSTALL_CONTEXT:
+#ifdef __riscos__
+      fixup_stack (&entry_vrs->core);
+#endif
       /* Upload the registers to enter the landing pad.  */
       uw_restore_core_regs (entry_vrs, &entry_vrs->core);
 
@@ -516,8 +587,12 @@
   if (!UCB_FORCED_STOP_FN (ucbp))
     return __gnu_Unwind_RaiseException (ucbp, entry_vrs);
 
+#ifdef __riscos__
+  ucbp->alloca_fp = VRS_FP (entry_vrs);
+#endif
+
   /* Set the pc to the call site.  */
-  VRS_PC (entry_vrs) = VRS_RETURN (entry_vrs);
+  VRS_PC (entry_vrs) = VRS_RETURN_NO_PSR (entry_vrs);
   /* Continue unwinding the next frame.  */
   return unwind_phase2_forced (ucbp, entry_vrs, 0);
 }
@@ -554,7 +629,7 @@
   _Unwind_Control_Block *ucbp = &ucb;
 
   /* Set the pc to the call site.  */
-  VRS_PC (entry_vrs) = VRS_RETURN (entry_vrs);
+  VRS_PC (entry_vrs) = VRS_RETURN_NO_PSR (entry_vrs);
 
   /* Save the core registers.  */
   saved_vrs.core = entry_vrs->core;
@@ -563,8 +638,16 @@
   
   do
     {
+#ifdef __riscos__
+      __ehs_unwind_stack_chunk ((void **)&VRS_FP (&saved_vrs),
+				(void *)ucbp->alloca_fp,
+				(void **)&VRS_PC (&saved_vrs),
+				(void **)&VRS_SL (&saved_vrs));
+      ucbp->alloca_fp = VRS_FP (&saved_vrs);
+#endif
+
       /* Find the entry for this routine.  */
-      if (get_eit_entry (ucbp, VRS_PC (&saved_vrs)) != _URC_OK)
+      if (get_eit_entry (ucbp, VRS_PC_NO_PSR (&saved_vrs)) != _URC_OK)
 	{
 	  code = _URC_FAILURE;
 	  break;
