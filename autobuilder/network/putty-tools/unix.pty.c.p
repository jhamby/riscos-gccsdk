--- unix/pty.c.orig	2022-08-14 07:46:22.868731621 +0100
+++ unix/pty.c	2022-08-14 07:46:40.944661276 +0100
@@ -395,6 +395,7 @@
     }
 #endif
 
+#ifndef __riscos__
     if (grantpt(pty->master_fd) < 0) {
         perror("grantpt");
         exit(1);
@@ -410,6 +411,7 @@
     pty->name[FILENAME_MAX-1] = '\0';
     strncpy(pty->name, ptsname(pty->master_fd), FILENAME_MAX-1);
 #endif
+#endif
 
 #ifndef SET_NONBLOCK_VIA_OPENPT
     nonblock(pty->master_fd);
@@ -1063,7 +1063,7 @@
             close(slavefd);
             setsid();
 #ifdef TIOCSCTTY
-            ioctl(0, TIOCSCTTY, 1);
+            ioctl(0, TIOCSCTTY, (void *)1);
 #endif
             tcsetpgrp(0, pgrp);
 
