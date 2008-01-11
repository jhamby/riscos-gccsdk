--- zvt/gnome-utmp.c.old	2003-04-25 14:26:43.000000000 +0100
+++ zvt/gnome-utmp.c	2003-04-25 13:34:50.000000000 +0100
@@ -33,7 +33,7 @@
 #  endif
 #endif
 
-#include <utmp.h>
+//#include <utmp.h>
 
 #if defined(HAVE_LASTLOG_H)
 #    include <lastlog.h>
@@ -127,7 +127,7 @@
 #endif /* HAVE_FCNTL || HAVE_FLOCK */
 
 	lseek (fd, 0, SEEK_END);
-	write (fd, putmp, sizeof(UTMP));
+//	write (fd, putmp, sizeof(UTMP));
 
 	/* unlock the file */
 #if defined(HAVE_FCNTL)	
@@ -221,7 +221,7 @@
 void 
 write_logout_record (void *data, int utmp, int wtmp)
 {
-	UTMP put, *ut = data;
+/*	UTMP put, *ut = data;
 
 	memset (&put, 0, sizeof(UTMP));
 
@@ -246,7 +246,7 @@
 	if (wtmp)
 		update_wtmp (WTMP_OUTPUT_FILENAME, &put);
 
-	free (ut);
+	free (ut);*/
 }
 
 void *
@@ -255,10 +255,10 @@
 	UTMP *ut;
 	char *pty = term_name;
 
-	if((ut=(UTMP *) malloc (sizeof (UTMP))) == NULL)
+//	if((ut=(UTMP *) malloc (sizeof (UTMP))) == NULL)
 		return NULL;
 	
-	memset (ut, 0, sizeof (UTMP));
+//	memset (ut, 0, sizeof (UTMP));
 
 #if defined(HAVE_UT_UT_NAME)
 	strncpy (ut->ut_name, login_name, sizeof (ut->ut_name));
@@ -303,8 +303,8 @@
 
 	/* For utmpx ut_line should be null terminated */
 	/* We do that for both cases to be sure */
-	strncpy (ut->ut_line, pty, sizeof (ut->ut_line));
-	ut->ut_line[sizeof (ut->ut_line)-1] = '\0';
+//	strncpy (ut->ut_line, pty, sizeof (ut->ut_line));
+//	ut->ut_line[sizeof (ut->ut_line)-1] = '\0';
 
 	/* We want parent's pid, not our own */
 #if defined(HAVE_UT_UT_PID)
@@ -332,8 +332,8 @@
 	if (utmp)
 		update_utmp (ut);
 
-	if (wtmp)
-		update_wtmp (WTMP_OUTPUT_FILENAME, ut);
+//	if (wtmp)
+//		update_wtmp (WTMP_OUTPUT_FILENAME, ut);
 
 	if (lastlog)
 		update_lastlog(login_name, ut);
--- zvt/gnome-pty-helper.c.old	2003-04-25 13:35:26.000000000 +0100
+++ zvt/gnome-pty-helper.c	2003-04-25 13:37:14.000000000 +0100
@@ -50,7 +50,7 @@
 #include <stdlib.h>
 #include <string.h>
 #include <stdio.h>
-#include <utmp.h>
+//#include <utmp.h>
 #include <grp.h>
 #include "gnome-pty.h"
 #include "gnome-login-support.h"
@@ -202,7 +202,7 @@
 }
 
 #else
-#include <stropts.h>
+//#include <stropts.h>
 static int
 init_msg_pass ()
 {
@@ -213,7 +213,7 @@
 int
 pass_fd (int client_fd, int fd)
 {
-	if (ioctl (client_fd, I_SENDFD, fd) < 0)
+//	if (ioctl (client_fd, I_SENDFD, fd) < 0)
 		return -1;
 	return 0;
 }
@@ -251,9 +251,9 @@
 static void
 shutdown_pty (pty_info *pi)
 {
-	if (pi->utmp || pi->wtmp)
-		if (pi->data)
-			write_logout_record (pi->data, pi->utmp, pi->wtmp);
+//	if (pi->utmp || pi->wtmp)
+//		if (pi->data)
+//			write_logout_record (pi->data, pi->utmp, pi->wtmp);
 	
 	close (pi->master_fd);
 	close (pi->slave_fd);
@@ -519,7 +519,7 @@
 #error "No means to drop privileges! Huge security risk! Won't compile."
 #endif
 	/* Open pty with priveleges of the user */
-	status = openpty (&master_pty, &slave_pty, term_name, &term, NULL);
+	status = -1; //openpty (&master_pty, &slave_pty, term_name, &term, NULL);
 
 	/* Restore saved priveleges to root */
 #ifdef HAVE_SETEUID
@@ -698,7 +698,7 @@
 		exit (1);
 
 	for (;;){
-		res = n_read (STDIN_FILENO, &op, sizeof (op));
+		res = -1; //n_read (STDIN_FILENO, &op, sizeof (op));
 
 		if (res != sizeof (op) || res == -1){
 			shutdown_helper ();
