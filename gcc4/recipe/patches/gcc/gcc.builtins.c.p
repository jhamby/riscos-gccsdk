Index: gcc/builtins.c
===================================================================
--- gcc/builtins.c	(revision 169861)
+++ gcc/builtins.c	(working copy)
@@ -670,6 +670,38 @@
 {
   int i;
 
+#if defined(HAVE_BUILTIN_RETURN_ADDR_FUNC) || defined(HAVE_BUILTIN_FRAME_ADDR_FUNC)
+  {
+    const char *func = NULL;
+
+    /* If HAVE_BUILTIN_RETURN_ADDR_FUNC or HAVE_BUILTIN_FRAME_ADDR_FUNC
+       are defined, and evaluate to something, then call
+       __builtin_return_address as a function.  */
+#ifdef HAVE_BUILTIN_RETURN_ADDR_FUNC
+    if (fndecl_code == BUILT_IN_RETURN_ADDRESS
+        && HAVE_BUILTIN_RETURN_ADDR_FUNC)
+      func = "__builtin_return_address";
+#endif
+#ifdef HAVE_BUILTIN_FRAME_ADDR_FUNC
+    if (fndecl_code == BUILT_IN_FRAME_ADDRESS
+        && HAVE_BUILTIN_FRAME_ADDR_FUNC)
+      func = "__builtin_frame_address";
+#endif
+
+    if (func != NULL)
+      {
+        rtx function_call;
+
+        rtx tem = gen_reg_rtx (Pmode);
+        function_call = gen_rtx_SYMBOL_REF (Pmode, func);
+        emit_library_call_value (function_call, tem, LCT_NORMAL,
+                                 Pmode, 1,
+                                 GEN_INT (count), SImode);
+        return tem;
+      }
+  }
+#endif
+
 #ifdef INITIAL_FRAME_ADDRESS_RTX
   rtx tem = INITIAL_FRAME_ADDRESS_RTX;
 #else
