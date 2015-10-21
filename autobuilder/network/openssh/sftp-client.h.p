--- ./sftp-client.h.orig	2015-09-23 16:04:35.421552334 +0200
+++ ./sftp-client.h	2015-09-23 16:04:35.437552334 +0200
@@ -30,24 +30,6 @@
 };
 
 /*
- * Used for statvfs responses on the wire from the server, because the
- * server's native format may be larger than the client's.
- */
-struct sftp_statvfs {
-	u_int64_t f_bsize;
-	u_int64_t f_frsize;
-	u_int64_t f_blocks;
-	u_int64_t f_bfree;
-	u_int64_t f_bavail;
-	u_int64_t f_files;
-	u_int64_t f_ffree;
-	u_int64_t f_favail;
-	u_int64_t f_fsid;
-	u_int64_t f_flag;
-	u_int64_t f_namemax;
-};
-
-/*
  * Initialise a SSH filexfer connection. Returns NULL on error or
  * a pointer to a initialized sftp_conn struct on success.
  */
@@ -88,9 +70,6 @@
 /* Canonicalise 'path' - caller must free result */
 char *do_realpath(struct sftp_conn *, const char *);
 
-/* Get statistics for filesystem hosting file at "path" */
-int do_statvfs(struct sftp_conn *, const char *, struct sftp_statvfs *, int);
-
 /* Rename 'oldpath' to 'newpath' */
 int do_rename(struct sftp_conn *, const char *, const char *, int force_legacy);
 
