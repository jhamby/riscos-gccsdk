--- sftp-client.c.orig	2021-01-19 18:35:19.072655722 +0000
+++ sftp-client.c	2021-01-19 18:40:34.966403349 +0000
@@ -24,9 +24,11 @@
 
 #include <sys/types.h>
 #include <sys/param.h>
+#ifndef __riscos__
 #ifdef HAVE_SYS_STATVFS_H
 #include <sys/statvfs.h>
 #endif
+#endif
 #include "openbsd-compat/sys-queue.h"
 #ifdef HAVE_SYS_STAT_H
 # include <sys/stat.h>
@@ -280,6 +282,8 @@
 	return(a);
 }
 
+#ifndef __riscos__
+
 static int
 get_decode_statvfs(struct sftp_conn *conn, struct sftp_statvfs *st,
     u_int expected_id, int quiet)
@@ -331,6 +335,8 @@
 	return 0;
 }
 
+#endif
+
 struct sftp_conn *
 do_init(int fd_in, int fd_out, u_int transfer_buflen, u_int num_requests,
     u_int64_t limit_kbps)
@@ -378,6 +384,7 @@
 		    strcmp(value, "1") == 0) {
 			ret->exts |= SFTP_EXT_POSIX_RENAME;
 			known = 1;
+#ifndef __riscos__
 		} else if (strcmp(name, "statvfs@openssh.com") == 0 &&
 		    strcmp(value, "2") == 0) {
 			ret->exts |= SFTP_EXT_STATVFS;
@@ -386,6 +393,7 @@
 		    strcmp(value, "2") == 0) {
 			ret->exts |= SFTP_EXT_FSTATVFS;
 			known = 1;
+#endif
 		} else if (strcmp(name, "hardlink@openssh.com") == 0 &&
 		    strcmp(value, "1") == 0) {
 			ret->exts |= SFTP_EXT_HARDLINK;
@@ -954,6 +962,7 @@
 }
 #endif
 
+#ifndef __riscos__
 int
 do_statvfs(struct sftp_conn *conn, const char *path, struct sftp_statvfs *st,
     int quiet)
@@ -1007,6 +1016,7 @@
 	return get_decode_statvfs(conn, st, id, quiet);
 }
 #endif
+#endif
 
 static void
 send_read_request(struct sftp_conn *conn, u_int id, u_int64_t offset,
