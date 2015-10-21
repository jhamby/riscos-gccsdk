--- sftp-server.c.orig	2015-10-21 18:16:03.412081644 +0100
+++ sftp-server.c	2015-10-21 18:16:10.784081564 +0100
@@ -26,9 +26,11 @@
 #ifdef HAVE_SYS_MOUNT_H
 #include <sys/mount.h>
 #endif
+#ifndef __riscos__
 #ifdef HAVE_SYS_STATVFS_H
 #include <sys/statvfs.h>
 #endif
+#endif
 #ifdef HAVE_SYS_PRCTL_H
 #include <sys/prctl.h>
 #endif
@@ -107,8 +109,10 @@
 static void process_readlink(u_int32_t id);
 static void process_symlink(u_int32_t id);
 static void process_extended_posix_rename(u_int32_t id);
+#ifndef __riscos__
 static void process_extended_statvfs(u_int32_t id);
 static void process_extended_fstatvfs(u_int32_t id);
+#endif
 static void process_extended_hardlink(u_int32_t id);
 static void process_extended_fsync(u_int32_t id);
 static void process_extended(u_int32_t id);
@@ -148,8 +152,10 @@
 struct sftp_handler extended_handlers[] = {
 	{ "posix-rename", "posix-rename@openssh.com", 0,
 	   process_extended_posix_rename, 1 },
+#ifndef __riscos__
 	{ "statvfs", "statvfs@openssh.com", 0, process_extended_statvfs, 0 },
 	{ "fstatvfs", "fstatvfs@openssh.com", 0, process_extended_fstatvfs, 0 },
+#endif
 	{ "hardlink", "hardlink@openssh.com", 0, process_extended_hardlink, 1 },
 	{ "fsync", "fsync@openssh.com", 0, process_extended_fsync, 1 },
 	{ NULL, NULL, 0, NULL, 0 }
@@ -1296,7 +1302,7 @@
 	free(oldpath);
 	free(newpath);
 }
-
+#ifndef __riscos__
 static void
 process_extended_statvfs(u_int32_t id)
 {
@@ -1335,7 +1341,7 @@
 	else
 		send_statvfs(id, &st);
 }
-
+#endif
 static void
 process_extended_hardlink(u_int32_t id)
 {
