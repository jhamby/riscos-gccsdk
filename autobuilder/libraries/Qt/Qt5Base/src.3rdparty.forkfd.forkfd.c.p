--- src/3rdparty/forkfd/forkfd.c.orig	2020-10-27 01:02:11.000000000 -0700
+++ src/3rdparty/forkfd/forkfd.c	2021-08-22 16:56:07.032999506 -0700
@@ -240,6 +240,7 @@
     }
 }
 
+#if 0
 static int convertForkfdWaitFlagsToWaitFlags(int ffdoptions)
 {
     int woptions = WEXITED;
@@ -249,6 +250,7 @@
         woptions |= WNOHANG;
     return woptions;
 }
+#endif
 
 static int tryReaping(pid_t pid, struct pipe_payload *payload)
 {
@@ -763,7 +765,7 @@
 }
 #endif // FORKFD_NO_FORKFD
 
-#if _POSIX_SPAWN > 0 && !defined(FORKFD_NO_SPAWNFD)
+#if _POSIX_SPAWN > 0 && !defined(FORKFD_NO_SPAWNFD) && !defined(Q_OS_RISCOS)
 int spawnfd(int flags, pid_t *ppid, const char *path, const posix_spawn_file_actions_t *file_actions,
             posix_spawnattr_t *attrp, char *const argv[], char *const envp[])
 {
