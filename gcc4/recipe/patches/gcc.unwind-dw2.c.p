--- gcc/unwind-dw2.c.orig	2009-06-07 11:02:55.000000000 +0100
+++ gcc/unwind-dw2.c	2009-06-07 10:59:47.000000000 +0100
@@ -60,6 +60,10 @@
 #define DWARF_REG_TO_UNWIND_COLUMN(REGNO) (REGNO)
 #endif
 
+#ifdef TARGET_RISCOSELF
+void *__ehs_return_address (void *fp);
+#endif
+
 /* This is the register and unwind state for a particular frame.  This
    provides the information necessary to unwind up past a frame and return
    to its caller.  */
@@ -71,6 +75,9 @@ struct _Unwind_Context
   void *lsda;
   struct dwarf_eh_bases bases;
   _Unwind_Word args_size;
+#ifdef TARGET_RISCOSELF
+  void *sl;
+#endif
 };
 
 /* Byte size of every register managed by these routines.  */
@@ -1201,7 +1208,6 @@ uw_update_context_1 (struct _Unwind_Cont
    of its caller.  Update CONTEXT to refer to the caller as well.  Note
    that the args_size and lsda members are not updated here, but later in
    uw_frame_state_for.  */
-
 static void
 uw_update_context (struct _Unwind_Context *context, _Unwind_FrameState *fs)
 {
@@ -1209,8 +1215,15 @@ uw_update_context (struct _Unwind_Contex
 
   /* Compute the return address now, since the return address column
      can change from frame to frame.  */
+#ifdef TARGET_RISCOSELF
+  /* If the return address is __free_stack_chunk or __gcc_alloca_free,
+     then find the real return address.  */
+  context->ra = __builtin_extract_return_addr
+    (__ehs_return_address(context->cfa - 4));
+#else
   context->ra = __builtin_extract_return_addr
     (_Unwind_GetPtr (context, fs->retaddr_column));
+#endif
 }
 
 static void
@@ -1222,6 +1235,21 @@ uw_advance_context (struct _Unwind_Conte
 /* Fill in CONTEXT for top-of-stack.  The only valid registers at this
    level will be the return address and the CFA.  */
 
+#ifdef TARGET_RISCOSELF
+/* The context doesn't include the stack limit register, so we must store
+   it ourselves.  */
+#define uw_init_context(CONTEXT)					   \
+  do									   \
+    {									   \
+      register void *sl asm("r10");					   \
+      /* Do any necessary initialization to access arbitrary stack frames. \
+	 On the SPARC, this means flushing the register windows.  */	   \
+      __builtin_unwind_init ();						   \
+      uw_init_context_1 (CONTEXT, __builtin_dwarf_cfa (),		   \
+			 __builtin_return_address (0), sl);		   \
+    }									   \
+  while (0)
+#else
 #define uw_init_context(CONTEXT)					   \
   do									   \
     {									   \
@@ -1232,6 +1260,7 @@ uw_advance_context (struct _Unwind_Conte
 			 __builtin_return_address (0));			   \
     }									   \
   while (0)
+#endif
 
 static inline void
 init_dwarf_reg_size_table (void)
@@ -1241,7 +1270,11 @@ init_dwarf_reg_size_table (void)
 
 static void
 uw_init_context_1 (struct _Unwind_Context *context,
-		   void *outer_cfa, void *outer_ra)
+		   void *outer_cfa, void *outer_ra
+#ifdef TARGET_RISCOSELF
+		   ,void *sl
+#endif
+		   )
 {
   void *ra = __builtin_extract_return_addr (__builtin_return_address (0));
   _Unwind_FrameState fs;
@@ -1249,6 +1282,11 @@ uw_init_context_1 (struct _Unwind_Contex
   _Unwind_Reason_Code code;
 
   memset (context, 0, sizeof (struct _Unwind_Context));
+
+#ifdef TARGET_RISCOSELF
+  context->sl = sl;
+#endif
+
   context->ra = ra;
 
   code = uw_frame_state_for (context, &fs);
@@ -1284,7 +1322,17 @@ uw_init_context_1 (struct _Unwind_Contex
 /* Install TARGET into CURRENT so that we can return to it.  This is a
    macro because __builtin_eh_return must be invoked in the context of
    our caller.  */
-
+#ifdef TARGET_RISCOSELF
+#define uw_install_context(CURRENT, TARGET)				 \
+  do									 \
+    {									 \
+      long offset = uw_install_context_1 ((CURRENT), (TARGET));		 \
+      void *handler = __builtin_frob_return_addr ((TARGET)->ra);	 \
+      asm volatile ("LDR sl, %0\n" : : "m" ((TARGET)->sl));		 \
+      __builtin_eh_return (offset, handler);				 \
+    }									 \
+  while (0)
+#else
 #define uw_install_context(CURRENT, TARGET)				 \
   do									 \
     {									 \
@@ -1293,6 +1341,7 @@ uw_init_context_1 (struct _Unwind_Contex
       __builtin_eh_return (offset, handler);				 \
     }									 \
   while (0)
+#endif
 
 static long
 uw_install_context_1 (struct _Unwind_Context *current,
