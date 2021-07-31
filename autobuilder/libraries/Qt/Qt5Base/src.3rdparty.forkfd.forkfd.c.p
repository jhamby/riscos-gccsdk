--- src/3rdparty/forkfd/forkfd.c.orig	2015-06-29 21:04:35.000000000 +0100
+++ src/3rdparty/forkfd/forkfd.c	2015-07-19 14:11:09.081150683 +0100
@@ -620,7 +620,7 @@
 }
 #endif // FORKFD_NO_FORKFD
 
-#if defined(_POSIX_SPAWN) && !defined(FORKFD_NO_SPAWNFD)
+#if defined(_POSIX_SPAWN) && !defined(FORKFD_NO_SPAWNFD) && !defined(__riscos__)
 int spawnfd(int flags, pid_t *ppid, const char *path, const posix_spawn_file_actions_t *file_actions,
             posix_spawnattr_t *attrp, char *const argv[], char *const envp[])
 {
