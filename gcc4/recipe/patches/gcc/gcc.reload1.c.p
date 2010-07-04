--- gcc/reload1.c.orig	2006-06-14 11:24:44.000000000 +0200
+++ gcc/reload1.c	2010-07-04 19:34:10.076901864 +0200
@@ -3547,6 +3547,7 @@ init_elim_table (void)
   /* Does this function require a frame pointer?  */
 
   frame_pointer_needed = (! flag_omit_frame_pointer
+#ifndef TARGET_RISCOSELF
 			  /* ?? If EXIT_IGNORE_STACK is set, we will not save
 			     and restore sp for alloca.  So we can't eliminate
 			     the frame pointer in that case.  At some point,
@@ -3554,6 +3555,7 @@ init_elim_table (void)
 			     sp-adjusting insns for this case.  */
 			  || (current_function_calls_alloca
 			      && EXIT_IGNORE_STACK)
+#endif
 			  || current_function_accesses_prior_frames
 			  || FRAME_POINTER_REQUIRED);
 
