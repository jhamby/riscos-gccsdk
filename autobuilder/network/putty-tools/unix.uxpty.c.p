--- unix/uxpty.c.orig	2020-08-10 12:51:21.196143649 +0100
+++ unix/uxpty.c	2020-08-10 12:50:54.536650869 +0100
@@ -13,7 +13,9 @@
 #include <fcntl.h>
 #include <termios.h>
 #include <grp.h>
+#ifndef __riscos__
 #include <utmp.h>
+#endif
 #include <pwd.h>
 #include <time.h>
 #include <sys/types.h> 
@@ -1060,7 +1063,7 @@
             close(slavefd);
             setsid();
 #ifdef TIOCSCTTY
-	ioctl(0, TIOCSCTTY, 1);
+	ioctl(0, TIOCSCTTY, (void *)1);
 #endif
             tcsetpgrp(0, pgrp);
 
