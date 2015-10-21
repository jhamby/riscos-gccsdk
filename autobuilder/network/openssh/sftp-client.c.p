--- ./sftp-client.c.orig	2015-09-23 16:04:34.973552339 +0200
+++ ./sftp-client.c	2015-09-23 16:04:34.989552339 +0200
@@ -24,9 +24,6 @@
 
 #include <sys/param.h>	/* MIN MAX */
 #include <sys/types.h>
-#ifdef HAVE_SYS_STATVFS_H
-#include <sys/statvfs.h>
-#endif
 #include "openbsd-compat/sys-queue.h"
 #ifdef HAVE_SYS_STAT_H
 # include <sys/stat.h>
@@ -313,65 +310,6 @@
 	return &a;
 }
 
-static int
-get_decode_statvfs(struct sftp_conn *conn, struct sftp_statvfs *st,
-    u_int expected_id, int quiet)
-{
-	struct sshbuf *msg;
-	u_char type;
-	u_int id;
-	u_int64_t flag;
-	int r;
-
-	if ((msg = sshbuf_new()) == NULL)
-		fatal("%s: sshbuf_new failed", __func__);
-	get_msg(conn, msg);
-
-	if ((r = sshbuf_get_u8(msg, &type)) != 0 ||
-	    (r = sshbuf_get_u32(msg, &id)) != 0)
-		fatal("%s: buffer error: %s", __func__, ssh_err(r));
-
-	debug3("Received statvfs reply T:%u I:%u", type, id);
-	if (id != expected_id)
-		fatal("ID mismatch (%u != %u)", id, expected_id);
-	if (type == SSH2_FXP_STATUS) {
-		u_int status;
-
-		if ((r = sshbuf_get_u32(msg, &status)) != 0)
-			fatal("%s: buffer error: %s", __func__, ssh_err(r));
-		if (quiet)
-			debug("Couldn't statvfs: %s", fx2txt(status));
-		else
-			error("Couldn't statvfs: %s", fx2txt(status));
-		sshbuf_free(msg);
-		return -1;
-	} else if (type != SSH2_FXP_EXTENDED_REPLY) {
-		fatal("Expected SSH2_FXP_EXTENDED_REPLY(%u) packet, got %u",
-		    SSH2_FXP_EXTENDED_REPLY, type);
-	}
-
-	memset(st, 0, sizeof(*st));
-	if ((r = sshbuf_get_u64(msg, &st->f_bsize)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_frsize)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_blocks)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_bfree)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_bavail)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_files)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_ffree)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_favail)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_fsid)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &flag)) != 0 ||
-	    (r = sshbuf_get_u64(msg, &st->f_namemax)) != 0)
-		fatal("%s: buffer error: %s", __func__, ssh_err(r));
-
-	st->f_flag = (flag & SSH2_FXE_STATVFS_ST_RDONLY) ? ST_RDONLY : 0;
-	st->f_flag |= (flag & SSH2_FXE_STATVFS_ST_NOSUID) ? ST_NOSUID : 0;
-
-	sshbuf_free(msg);
-
-	return 0;
-}
-
 struct sftp_conn *
 do_init(int fd_in, int fd_out, u_int transfer_buflen, u_int num_requests,
     u_int64_t limit_kbps)
@@ -430,14 +368,6 @@
 		    strcmp((char *)value, "1") == 0) {
 			ret->exts |= SFTP_EXT_POSIX_RENAME;
 			known = 1;
-		} else if (strcmp(name, "statvfs@openssh.com") == 0 &&
-		    strcmp((char *)value, "2") == 0) {
-			ret->exts |= SFTP_EXT_STATVFS;
-			known = 1;
-		} else if (strcmp(name, "fstatvfs@openssh.com") == 0 &&
-		    strcmp((char *)value, "2") == 0) {
-			ret->exts |= SFTP_EXT_FSTATVFS;
-			known = 1;
 		} else if (strcmp(name, "hardlink@openssh.com") == 0 &&
 		    strcmp((char *)value, "1") == 0) {
 			ret->exts |= SFTP_EXT_HARDLINK;
@@ -1061,65 +991,6 @@
 }
 #endif
 
-int
-do_statvfs(struct sftp_conn *conn, const char *path, struct sftp_statvfs *st,
-    int quiet)
-{
-	struct sshbuf *msg;
-	u_int id;
-	int r;
-
-	if ((conn->exts & SFTP_EXT_STATVFS) == 0) {
-		error("Server does not support statvfs@openssh.com extension");
-		return -1;
-	}
-
-	id = conn->msg_id++;
-
-	if ((msg = sshbuf_new()) == NULL)
-		fatal("%s: sshbuf_new failed", __func__);
-	sshbuf_reset(msg);
-	if ((r = sshbuf_put_u8(msg, SSH2_FXP_EXTENDED)) != 0 ||
-	    (r = sshbuf_put_u32(msg, id)) != 0 ||
-	    (r = sshbuf_put_cstring(msg, "statvfs@openssh.com")) != 0 ||
-	    (r = sshbuf_put_cstring(msg, path)) != 0)
-		fatal("%s: buffer error: %s", __func__, ssh_err(r));
-	send_msg(conn, msg);
-	sshbuf_free(msg);
-
-	return get_decode_statvfs(conn, st, id, quiet);
-}
-
-#ifdef notyet
-int
-do_fstatvfs(struct sftp_conn *conn, const u_char *handle, u_int handle_len,
-    struct sftp_statvfs *st, int quiet)
-{
-	struct sshbuf *msg;
-	u_int id;
-
-	if ((conn->exts & SFTP_EXT_FSTATVFS) == 0) {
-		error("Server does not support fstatvfs@openssh.com extension");
-		return -1;
-	}
-
-	id = conn->msg_id++;
-
-	if ((msg = sshbuf_new()) == NULL)
-		fatal("%s: sshbuf_new failed", __func__);
-	sshbuf_reset(msg);
-	if ((r = sshbuf_put_u8(msg, SSH2_FXP_EXTENDED)) != 0 ||
-	    (r = sshbuf_put_u32(msg, id)) != 0 ||
-	    (r = sshbuf_put_cstring(msg, "fstatvfs@openssh.com")) != 0 ||
-	    (r = sshbuf_put_string(msg, handle, handle_len)) != 0)
-		fatal("%s: buffer error: %s", __func__, ssh_err(r));
-	send_msg(conn, msg);
-	sshbuf_free(msg);
-
-	return get_decode_statvfs(conn, st, id, quiet);
-}
-#endif
-
 static void
 send_read_request(struct sftp_conn *conn, u_int id, u_int64_t offset,
     u_int len, const u_char *handle, u_int handle_len)
