--- gcc/ira.c.orig	2017-02-06 13:27:19.000000000 +0000
+++ gcc/ira.c	2017-02-06 13:27:35.978256456 +0000
@@ -2262,7 +2262,9 @@
      sp-adjusting insns for this case.  */
   frame_pointer_needed
     = (! flag_omit_frame_pointer
+#ifndef TARGET_RISCOSELF
        || (cfun->calls_alloca && EXIT_IGNORE_STACK)
+#endif
        /* We need the frame pointer to catch stack overflow exceptions if
 	  the stack pointer is moving (as for the alloca case just above).  */
        || (STACK_CHECK_MOVING_SP
