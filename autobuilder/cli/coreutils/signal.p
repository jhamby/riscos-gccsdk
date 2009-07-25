--- lib/signal.in.h.orig	2009-07-25 10:00:16.000000000 -0700
+++ lib/signal.in.h	2009-07-25 10:00:46.000000000 -0700
@@ -191,6 +191,9 @@
 #ifndef SA_NODEFER
 # define SA_NODEFER 0
 #endif
+#ifndef SA_RESETHAND
+# define SA_RESETHAND 0
+#endif
 
 
 #ifdef __cplusplus
