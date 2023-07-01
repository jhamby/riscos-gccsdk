--- Modules/faulthandler.c.orig	2023-07-01 11:26:03.658422740 +0100
+++ Modules/faulthandler.c	2023-07-01 11:27:11.137030362 +0100
@@ -21,7 +21,7 @@
 
 /* Using an alternative stack requires sigaltstack()
    and sigaction() SA_ONSTACK */
-#if defined(HAVE_SIGALTSTACK) && defined(HAVE_SIGACTION)
+#if defined(HAVE_SIGALTSTACK) && defined(HAVE_SIGACTION) && !defined(__riscos__)
 #  define FAULTHANDLER_USE_ALT_STACK
 #endif
 
@@ -509,7 +509,9 @@
         sigemptyset(&action.sa_mask);
         /* Do not prevent the signal from being received from within
            its own signal handler */
+#ifndef __riscos__
         action.sa_flags = SA_NODEFER;
+#endif
 #ifdef FAULTHANDLER_USE_ALT_STACK
         assert(stack.ss_sp != NULL);
         /* Call the signal handler on an alternate signal stack
@@ -820,11 +822,13 @@
        call, try to restart the system call instead of interrupting it and
        return EINTR. */
     action.sa_flags = SA_RESTART;
+#ifndef __riscos__
     if (chain) {
         /* do not prevent the signal from being received from within its
            own signal handler */
         action.sa_flags = SA_NODEFER;
     }
+#endif
 #ifdef FAULTHANDLER_USE_ALT_STACK
     assert(stack.ss_sp != NULL);
     /* Call the signal handler on an alternate signal stack
