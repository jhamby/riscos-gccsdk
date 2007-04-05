--- unix/uxpty.c.orig	2007-04-03 11:47:53.000000000 +0100
+++ unix/uxpty.c	2007-04-03 12:00:42.000000000 +0100
@@ -13,7 +13,7 @@
 #include <fcntl.h>
 #include <termios.h>
 #include <grp.h>
-#include <utmp.h>
+//#include <utmp.h>
 #include <pwd.h>
 #include <time.h>
 #include <sys/types.h>
@@ -260,7 +260,8 @@
 
 static void sigchld_handler(int signum)
 {
-    write(pty_signal_pipe[1], "x", 1);
+    int count;
+    count = write(pty_signal_pipe[1], "x", 1);
 }
 
 #ifndef OMIT_UTMP
@@ -291,6 +292,7 @@
     const char *p1, *p2;
     char master_name[20];
     struct group *gp;
+    int success;
 
     for (p1 = chars1; *p1; p1++)
 	for (p2 = chars2; *p2; p2++) {
@@ -330,7 +332,7 @@
 
     /* We need to chown/chmod the /dev/ttyXX device. */
     gp = getgrnam("tty");
-    chown(pty->name, getuid(), gp ? gp->gr_gid : -1);
+    success = chown(pty->name, getuid(), gp ? gp->gr_gid : -1);
     chmod(pty->name, 0600);
 #else
     pty->master_fd = open("/dev/ptmx", O_RDWR);
@@ -630,8 +632,9 @@
 	int ipid;
 	int status;
 	char c[1];
+	int count;
 
-	read(pty_signal_pipe[0], c, 1); /* ignore its value; it'll be `x' */
+	count = read(pty_signal_pipe[0], c, 1); /* ignore its value; it'll be `x' */
 
 	do {
 	    pid = waitpid(-1, &status, WNOHANG);
@@ -775,7 +778,7 @@
 	close(slavefd);
 	setsid();
 #ifdef TIOCSCTTY
-	ioctl(0, TIOCSCTTY, 1);
+	ioctl(0, TIOCSCTTY, (void *)1);
 #endif
 	pgrp = getpid();
 	tcsetpgrp(0, pgrp);
