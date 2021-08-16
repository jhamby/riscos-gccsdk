--- src/config.h.orig	2021-08-08 09:29:28.732087903 +0100
+++ src/config.h	2021-08-08 09:31:34.097672719 +0100
@@ -67,7 +67,10 @@
 /* # define USE_PRIVATE_PATHS */
 #endif /* PRIVATE_USER_PATH */
 
-
+#ifdef __riscos__
+#undef PRIVATE_USER_PATH
+#define PRIVATE_USER_PATH "/<Choices$Write>"
+#endif
 
 /*** Some no-brainer defines ***/
 
