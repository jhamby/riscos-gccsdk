--- unix/sftpserver.c.orig	2022-08-14 08:01:27.156214740 +0100
+++ unix/sftpserver.c	2022-08-14 08:02:25.227886273 +0100
@@ -602,7 +602,7 @@
         char *longnamebuf = NULL;
         struct fxp_attrs attrs = no_attrs;
 
-#if defined HAVE_FSTATAT && defined HAVE_DIRFD
+#if defined HAVE_FSTATAT && defined HAVE_DIRFD && !defined(__riscos__)
         struct stat st;
         if (!fstatat(dirfd(udh->dp), de->d_name, &st, AT_SYMLINK_NOFOLLOW)) {
             char perms[11], *uidbuf = NULL, *gidbuf = NULL;
