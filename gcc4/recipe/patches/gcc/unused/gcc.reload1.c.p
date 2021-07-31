--- gcc/reload1.c.orig	2006-09-24 21:49:42.000000000 +0200
+++ gcc/reload1.c	2006-09-24 21:58:38.000000000 +0200
@@ -3545,6 +3545,7 @@
   /* Does this function require a frame pointer?  */
 
   frame_pointer_needed = (! flag_omit_frame_pointer
+#ifndef TARGET_RISCOSELF
 			  /* ?? If EXIT_IGNORE_STACK is set, we will not save
 			     and restore sp for alloca.  So we can't eliminate
 			     the frame pointer in that case.  At some point,
@@ -3552,6 +3553,7 @@
 			     sp-adjusting insns for this case.  */
 			  || (current_function_calls_alloca
 			      && EXIT_IGNORE_STACK)
+#endif
 			  || current_function_accesses_prior_frames
 			  || FRAME_POINTER_REQUIRED);
 
