--- unix/uxpty.c.orig	2009-08-26 17:13:46.000000000 -0700
+++ unix/uxpty.c	2009-08-26 17:13:46.000000000 -0700
@@ -13,7 +13,7 @@
 #include <fcntl.h>
 #include <termios.h>
 #include <grp.h>
-#include <utmp.h>
+//#include <utmp.h>
 #include <pwd.h>
 #include <time.h>
 #include <sys/types.h>
@@ -292,6 +292,7 @@
     const char *p1, *p2;
     char master_name[20];
     struct group *gp;
+    int success;
 
     for (p1 = chars1; *p1; p1++)
 	for (p2 = chars2; *p2; p2++) {
@@ -331,7 +332,7 @@
 
     /* We need to chown/chmod the /dev/ttyXX device. */
     gp = getgrnam("tty");
-    chown(pty->name, getuid(), gp ? gp->gr_gid : -1);
+    success = chown(pty->name, getuid(), gp ? gp->gr_gid : -1);
     chmod(pty->name, 0600);
 #else
     pty->master_fd = open("/dev/ptmx", O_RDWR);
@@ -780,7 +781,7 @@
 	close(slavefd);
 	setsid();
 #ifdef TIOCSCTTY
-	ioctl(0, TIOCSCTTY, 1);
+	ioctl(0, TIOCSCTTY, (void *)1);
 #endif
 	pgrp = getpid();
 	tcsetpgrp(0, pgrp);
