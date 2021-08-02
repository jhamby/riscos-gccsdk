--- src/3rdparty/forkfd/forkfd.c.orig	2021-07-29 17:54:02.450209758 -0700
+++ src/3rdparty/forkfd/forkfd.c	2021-07-29 18:19:43.009793333 -0700
@@ -766,7 +766,7 @@
 }
 #endif // FORKFD_NO_FORKFD
 
-#if _POSIX_SPAWN > 0 && !defined(FORKFD_NO_SPAWNFD)
+#if _POSIX_SPAWN > 0 && !defined(FORKFD_NO_SPAWNFD) && !defined(__riscos__)
 int spawnfd(int flags, pid_t *ppid, const char *path, const posix_spawn_file_actions_t *file_actions,
             posix_spawnattr_t *attrp, char *const argv[], char *const envp[])
 {
