Index: libgcc/unwind-arm-common.inc
===================================================================
--- libgcc/unwind-arm-common.inc	(revision 182584)
+++ libgcc/unwind-arm-common.inc	(working copy)
@@ -101,8 +101,9 @@
 extern int __data_start;
 
 /* The exception index table location.  */
-extern __EIT_entry __exidx_start;
-extern __EIT_entry __exidx_end;
+/* RISC OS: These must be const so that in module code, they are not relocated.  */
+extern const __EIT_entry __exidx_start;
+extern const __EIT_entry __exidx_end;
 
 /* Core unwinding functions.  */
 
@@ -284,6 +285,14 @@
 
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
       if (get_eit_entry (ucbp, VRS_PC(vrs)) != _URC_OK)
 	abort ();
@@ -299,6 +308,10 @@
   if (pr_result != _URC_INSTALL_CONTEXT)
     abort();
 
+#ifdef __riscos__
+  fixup_stack (&vrs->core);
+#endif
+
   uw_restore_core_regs (vrs, &vrs->core);
 }
 
@@ -328,6 +341,14 @@
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
       entry_code = get_eit_entry (ucbp, VRS_PC (&saved_vrs));
 
@@ -384,6 +405,10 @@
       return _URC_FAILURE;
     }
 
+#ifdef __riscos__
+  fixup_stack (&saved_vrs.core);
+#endif
+
   uw_restore_core_regs (&saved_vrs, &saved_vrs.core);
 }
 
@@ -413,6 +438,10 @@
   phase1_vrs saved_vrs;
   _Unwind_Reason_Code pr_result;
 
+#ifdef __riscos__
+  ucbp->alloca_fp = VRS_FP (entry_vrs);
+#endif
+
   /* Set the pc to the call site.  */
   VRS_PC (entry_vrs) = VRS_RETURN(entry_vrs);
 
@@ -424,6 +453,14 @@
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
       if (get_eit_entry (ucbp, VRS_PC (&saved_vrs)) != _URC_OK)
 	return _URC_FAILURE;
@@ -487,6 +524,10 @@
       abort ();
     }
 
+#ifdef __riscos__
+  ucbp->alloca_fp = VRS_FP (entry_vrs);
+#endif
+
   /* Call the cached PR.  */
   pr_result = ((personality_routine) UCB_PR_ADDR (ucbp))
 	(_US_UNWIND_FRAME_RESUME, ucbp, (_Unwind_Context *) entry_vrs);
@@ -494,6 +535,9 @@
   switch (pr_result)
     {
     case _URC_INSTALL_CONTEXT:
+#ifdef __riscos__
+      fixup_stack (&entry_vrs->core);
+#endif
       /* Upload the registers to enter the landing pad.  */
       uw_restore_core_regs (entry_vrs, &entry_vrs->core);
 
@@ -516,6 +560,10 @@
   if (!UCB_FORCED_STOP_FN (ucbp))
     return __gnu_Unwind_RaiseException (ucbp, entry_vrs);
 
+#ifdef __riscos__
+  ucbp->alloca_fp = VRS_FP (entry_vrs);
+#endif
+
   /* Set the pc to the call site.  */
   VRS_PC (entry_vrs) = VRS_RETURN (entry_vrs);
   /* Continue unwinding the next frame.  */
@@ -563,6 +611,14 @@
   
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
       if (get_eit_entry (ucbp, VRS_PC (&saved_vrs)) != _URC_OK)
 	{
