--- vncviewer/shm.c.orig	2005-06-01 08:45:36.000000000 +0100
+++ vncviewer/shm.c	2005-06-01 08:45:49.000000000 +0100
@@ -22,6 +22,7 @@
  * shm.c - code to set up shared memory extension.
  */
 
+#ifdef MITSHM
 #include "vncviewer.h"
 #include <sys/ipc.h>
 #include <sys/shm.h>
@@ -99,3 +100,4 @@
 
   return image;
 }
+#endif
