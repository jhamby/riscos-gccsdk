--- src/3rdparty/forkfd/forkfd.h.orig	2015-06-29 21:04:35.000000000 +0100
+++ src/3rdparty/forkfd/forkfd.h	2015-07-19 14:14:32.637153321 +0100
@@ -28,10 +28,29 @@
 #include <fcntl.h>
 #include <unistd.h> // to get the POSIX flags
 
-#ifdef _POSIX_SPAWN
+#if defined(_POSIX_SPAWN) && !defined(__riscos__)
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
@@ -43,7 +62,7 @@
 
 int forkfd(int flags, pid_t *ppid);
 
-#ifdef _POSIX_SPAWN
+#if defined(_POSIX_SPAWN) && !defined(__riscos__)
 /* only for spawnfd: */
 #  define FFD_SPAWN_SEARCH_PATH   O_RDWR
 
