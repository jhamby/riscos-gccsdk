--- gcc/global.c.orig	2006-09-24 23:11:01.000000000 +0200
+++ gcc/global.c	2006-09-24 23:11:11.000000000 +0200
@@ -341,7 +341,9 @@
 #endif
   int need_fp
     = (! flag_omit_frame_pointer
+#ifndef TARGET_RISCOSELF
        || (current_function_calls_alloca && EXIT_IGNORE_STACK)
+#endif
        || FRAME_POINTER_REQUIRED);
 
   size_t i;
