--- glib/gslice.c.orig	2021-03-18 13:47:48.332693300 +0000
+++ glib/gslice.c	2022-01-14 14:45:34.342379375 +0000
@@ -434,6 +434,9 @@
       if (RUNNING_ON_VALGRIND)
         config->always_malloc = TRUE;
 #endif
+#ifdef __riscos__
+      config->always_malloc = TRUE;
+#endif
     }
 
   g_free (val_allocated);
