--- glib/gslice.c.orig	2020-07-30 22:06:53.000000000 +0100
+++ glib/gslice.c	2020-07-30 22:07:51.418321187 +0100
@@ -389,6 +389,9 @@
       if (RUNNING_ON_VALGRIND)
         config->always_malloc = TRUE;
 #endif
+#ifdef __riscos__
+      config->always_malloc = TRUE;
+#endif
     }
 }
 
