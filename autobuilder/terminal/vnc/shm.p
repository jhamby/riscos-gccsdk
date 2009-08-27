--- vncviewer/shm.c.orig	2000-06-11 05:00:51.000000000 -0700
+++ vncviewer/shm.c	2009-08-27 08:02:42.000000000 -0700
@@ -21,6 +21,7 @@
  * shm.c - code to set up shared memory extension.
  */
 
+#ifdef MITSHM
 #include <vncviewer.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
@@ -98,3 +99,4 @@
 
   return image;
 }
+#endif
