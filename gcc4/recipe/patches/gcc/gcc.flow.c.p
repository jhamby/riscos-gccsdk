--- gcc/flow.c.orig	2010-04-14 04:13:47.553234663 +0200
+++ gcc/flow.c	2010-04-14 04:12:33.073234223 +0200
@@ -1687,6 +1687,7 @@ propagate_one_insn (struct propagate_blo
 	 But if we are keeping the stack pointer depressed, we might well
 	 be deleting insns that are used to compute the amount to update
 	 it by, so they are fine.  */
+#if 0
       if (reload_completed
 	  && !(TREE_CODE (TREE_TYPE (current_function_decl)) == FUNCTION_TYPE
 		&& (TYPE_RETURNS_STACK_DEPRESSED
@@ -1697,6 +1698,7 @@ propagate_one_insn (struct propagate_blo
 		  && sibcall_epilogue_contains (insn)))
 	  && find_reg_note (insn, REG_MAYBE_DEAD, NULL_RTX) == 0)
 	fatal_insn ("Attempt to delete prologue/epilogue insn:", insn);
+#endif
 
       /* Record sets.  Do this even for dead instructions, since they
 	 would have killed the values if they hadn't been deleted.  To
