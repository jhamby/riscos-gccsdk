--- lib/signal.in.h.orig	2014-05-29 13:05:50.000000000 +0100
+++ lib/signal.in.h	2015-04-30 06:13:52.961051106 +0100
@@ -47,6 +47,13 @@
    us; so include <sys/types.h> now, before the second inclusion guard.  */
 #include <sys/types.h>
 
+#if defined __riscos__
+/* Hack required to stop pthread_t missing error on RISC OS */
+#undef __PTHREAD_H
+#define __need_pthread_t
+#include <pthread.h>
+#endif
+
 /* The include_next requires a split double-inclusion guard.  */
 #@INCLUDE_NEXT@ @NEXT_SIGNAL_H@
 
@@ -454,6 +461,9 @@
 #ifndef SA_NODEFER
 # define SA_NODEFER 0
 #endif
+#ifndef SA_RESETHAND
+# define SA_RESETHAND 0
+#endif
 
 
 #endif /* _@GUARD_PREFIX@_SIGNAL_H */
