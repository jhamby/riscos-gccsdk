--- lib/signal.in.h.orig	2017-01-01 02:54:41.000000000 +1300
+++ lib/signal.in.h	2020-09-08 01:05:53.357864237 +1200
@@ -456,7 +456,9 @@
 #ifndef SA_NODEFER
 # define SA_NODEFER 0
 #endif
-
+#ifndef SA_RESETHAND
+# define SA_RESETHAND 0
+#endif
 
 #endif /* _@GUARD_PREFIX@_SIGNAL_H */
 #endif /* _@GUARD_PREFIX@_SIGNAL_H */
