Index: gcc/unwind-dw2.c
===================================================================
--- gcc/unwind-dw2.c	(revision 162037)
+++ gcc/unwind-dw2.c	(working copy)
@@ -55,6 +55,12 @@
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
@@ -1394,8 +1400,26 @@
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
   uw_update_context_1 (context, fs);
 
+  /* If the return address is __free_stack_chunk or __gcc_alloca_free,
+     then find the real return address.  */
+  context->ra = __builtin_extract_return_addr
+    (__ehs_return_address(fp));
+#else
+  uw_update_context_1 (context, fs);
+
   /* In general this unwinder doesn't make any distinction between
      undefined and same_value rule.  Call-saved registers are assumed
      to have same_value rule by default and explicit undefined
@@ -1412,6 +1436,7 @@
        can change from frame to frame.  */
     context->ra = __builtin_extract_return_addr
       (_Unwind_GetPtr (context, fs->retaddr_column));
+#endif
 }
 
 static void
@@ -1500,15 +1525,43 @@
    macro because __builtin_eh_return must be invoked in the context of
    our caller.  */
 
+#ifdef TARGET_RISCOSELF
 #define uw_install_context(CURRENT, TARGET)				\
   do									\
     {									\
       long offset = uw_install_context_1 ((CURRENT), (TARGET));		\
       void *handler = __builtin_frob_return_addr ((TARGET)->ra);	\
+									\
+      /* vvv BEGIN RISC OS ELF */					\
+      /* The stack pointer of the target context */			\
+      void **sp = _Unwind_GetPtr (TARGET, 13);				\
+									\
+      /* Store the handler on the stack of the target context */	\
+      /* so that __ehs_trim_stack() can retrieve it later.  */		\
+      sp[-1] = handler;							\
+      handler = __ehs_trim_stack;					\
+									\
+      /* Find the stack limit for the target context so that it can */	\
+      /* be restored.  */						\
+      void *sl = __ehs_stack_limit(sp);	 				\
+      asm volatile ("MOV r10, %0\n" : : "r" (sl));			\
+      /* ^^^ END RISC OS ELF */						\
+									\
       _Unwind_DebugHook ((TARGET)->cfa, handler);			\
       __builtin_eh_return (offset, handler);				\
     }									\
   while (0)
+#else
+#define uw_install_context(CURRENT, TARGET)				\
+  do									\
+    {									\
+      long offset = uw_install_context_1 ((CURRENT), (TARGET));		\
+      void *handler = __builtin_frob_return_addr ((TARGET)->ra);	\
+      _Unwind_DebugHook ((TARGET)->cfa, handler);			\
+      __builtin_eh_return (offset, handler);				\
+    }									\
+  while (0)
+#endif
 
 static long
 uw_install_context_1 (struct _Unwind_Context *current,
