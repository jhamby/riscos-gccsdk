--- gcc/function.c.orig	2006-11-28 13:01:45.000000000 +0100
+++ gcc/function.c	2010-07-04 19:34:09.926901748 +0200
@@ -4387,6 +4387,18 @@ expand_function_end (void)
   force_next_line_note ();
   emit_line_note (input_location);
 
+  /* If we had calls to alloca, and this machine needs
+     an accurate stack pointer to exit the function,
+     insert some code to save and restore the stack pointer.  */
+  if (! EXIT_IGNORE_STACK
+      && current_function_calls_alloca)
+    {
+      rtx tem = 0;
+
+      emit_stack_save (SAVE_FUNCTION, &tem, parm_birth_insn);
+      emit_stack_restore (SAVE_FUNCTION, tem, NULL_RTX);
+    }
+
   /* Before the return label (if any), clobber the return
      registers so that they are not propagated live to the rest of
      the function.  This can only happen with functions that drop
