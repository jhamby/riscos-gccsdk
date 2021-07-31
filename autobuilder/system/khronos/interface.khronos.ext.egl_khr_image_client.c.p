--- interface/khronos/ext/egl_khr_image_client.c.orig	2016-12-07 20:46:45.000000000 +0000
+++ interface/khronos/ext/egl_khr_image_client.c	2016-12-21 21:13:45.462605121 +0000
@@ -75,6 +75,8 @@
 {
 #ifdef KHRONOS_HAVE_VCSM
     return true;
+#elif defined(__riscos__)
+    return false;
 #else
     static bool warn_once;
     bool success = false;
