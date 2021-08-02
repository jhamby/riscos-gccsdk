--- src/3rdparty/forkfd/forkfd.h.orig	2021-07-29 17:54:02.458209817 -0700
+++ src/3rdparty/forkfd/forkfd.h	2021-07-29 18:40:07.802965073 -0700
@@ -29,10 +29,29 @@
 #include <stdint.h>
 #include <unistd.h> // to get the POSIX flags
 
-#if _POSIX_SPAWN > 0
+#if _POSIX_SPAWN > 0 && !defined(__riscos__)
 #  include <spawn.h>
 #endif
 
+#ifdef __riscos__
+// Unixlib doesn't support these, so put them here.
+enum
+{
+  CLD_EXITED = 1,		/* Child has exited.  */
+# define CLD_EXITED	CLD_EXITED
+  CLD_KILLED,			/* Child was killed.  */
+# define CLD_KILLED	CLD_KILLED
+  CLD_DUMPED,			/* Child terminated abnormally.  */
+# define CLD_DUMPED	CLD_DUMPED
+  CLD_TRAPPED,			/* Traced child has trapped.  */
+# define CLD_TRAPPED	CLD_TRAPPED
+  CLD_STOPPED,			/* Child has stopped.  */
+# define CLD_STOPPED	CLD_STOPPED
+  CLD_CONTINUED			/* Stopped child has continued.  */
+# define CLD_CONTINUED	CLD_CONTINUED
+};
+#endif
+
 #ifdef __cplusplus
 extern "C" {
 #endif
@@ -51,7 +70,7 @@
 int forkfd_wait(int ffd, forkfd_info *info, struct rusage *rusage);
 int forkfd_close(int ffd);
 
-#if _POSIX_SPAWN > 0
+#if _POSIX_SPAWN > 0 && !defined(__riscos__)
 /* only for spawnfd: */
 #  define FFD_SPAWN_SEARCH_PATH   O_RDWR
 
