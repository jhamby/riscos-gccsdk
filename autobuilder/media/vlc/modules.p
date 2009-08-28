--- src/modules/modules.h.orig	2009-08-28 09:22:11.000000000 -0700
+++ src/modules/modules.h	2009-08-28 09:22:22.000000000 -0700
@@ -83,7 +83,7 @@
 typedef NSModule module_handle_t;
 #elif defined(HAVE_IMAGE_H)
 typedef int module_handle_t;
-#elif defined(WIN32) || defined(UNDER_CE)
+#elif defined(WIN32) || defined(UNDER_CE) || defined(__riscos__)
 typedef void * module_handle_t;
 #elif defined(HAVE_DL_DLOPEN)
 typedef void * module_handle_t;
