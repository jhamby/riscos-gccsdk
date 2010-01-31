--- gcc/unwind-dw2.c.orig	2009-06-07 11:02:55.000000000 +0100
+++ gcc/unwind-dw2.c	2009-06-14 15:58:11.000000000 +0100
@@ -60,6 +60,12 @@
 #define DWARF_REG_TO_UNWIND_COLUMN(REGNO) (REGNO)
 #endif
 
+#ifdef TARGET_RISCOSELF
+void *__ehs_return_address (void *fp);
+void *__ehs_stack_limit (void *sp);
+void __ehs_trim_stack(void);
+#endif
+
 /* This is the register and unwind state for a particular frame.  This
    provides the information necessary to unwind up past a frame and return
    to its caller.  */
@@ -1201,16 +1207,34 @@ uw_update_context_1 (struct _Unwind_Cont
    of its caller.  Update CONTEXT to refer to the caller as well.  Note
    that the args_size and lsda members are not updated here, but later in
    uw_frame_state_for.  */
-
 static void
 uw_update_context (struct _Unwind_Context *context, _Unwind_FrameState *fs)
 {
+#ifdef TARGET_RISCOSELF
+  /* I did try using context->cfa below as the frame pointer as r11
+     always seemed to be at a constant offset of -4 to it. However, there
+     are rare occasions when a function has an extra stack adjustment
+     instruction in the prologue that makes the offset -8 and it's
+     impossible to anticipate these.
+     Instead we extract r11 from the context, but we do it before the
+     context is updated as, for some reason, that gives the correct
+     frame pointer.  */
+  void *fp = _Unwind_GetPtr (context, 11);
+
+  uw_update_context_1 (context, fs);
+
+  /* If the return address is __free_stack_chunk or __gcc_alloca_free,
+     then find the real return address.  */
+  context->ra = __builtin_extract_return_addr
+    (__ehs_return_address(fp));
+#else
   uw_update_context_1 (context, fs);
 
   /* Compute the return address now, since the return address column
      can change from frame to frame.  */
   context->ra = __builtin_extract_return_addr
     (_Unwind_GetPtr (context, fs->retaddr_column));
+#endif
 }
 
 static void
@@ -1285,14 +1309,38 @@ uw_init_context_1 (struct _Unwind_Contex
    macro because __builtin_eh_return must be invoked in the context of
    our caller.  */
 
+#ifdef TARGET_RISCOSELF
 #define uw_install_context(CURRENT, TARGET)				 \
   do									 \
     {									 \
       long offset = uw_install_context_1 ((CURRENT), (TARGET));		 \
       void *handler = __builtin_frob_return_addr ((TARGET)->ra);	 \
+									 \
+      /* The stack pointer of the target context */			 \
+      void **sp = _Unwind_GetPtr (TARGET, 13);				 \
+									 \
+      /* Store the handler on the stack of the target context so that */ \
+      /* __ehs_trim_stack() can retrieve it later.  */			 \
+      sp[-1] = handler;							 \
+      handler = __ehs_trim_stack;					 \
+									 \
+      /* Find the stack limit for the target context so that it can */   \
+      /* be restored.  */						 \
+      void *sl = __ehs_stack_limit(sp);	 				 \
+      asm volatile ("MOV r10, %0\n" : : "r" (sl));			 \
       __builtin_eh_return (offset, handler);				 \
     }									 \
   while (0)
+#else
+#define uw_install_context(CURRENT, TARGET)				 \
+  do									 \
+    {									 \
+      long offset = uw_install_context_1 ((CURRENT), (TARGET));		 \
+      void *handler = __builtin_frob_return_addr ((TARGET)->ra);	 \
+      __builtin_eh_return (offset, handler);				 \
+    }									 \
+  while (0)
+#endif
 
 static long
 uw_install_context_1 (struct _Unwind_Context *current,
