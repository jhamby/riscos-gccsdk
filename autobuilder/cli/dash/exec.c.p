--- src/exec.c.orig	2014-09-28 21:19:32.000000000 +1300
+++ src/exec.c	2020-06-29 23:57:45.181438591 +1200
@@ -159,6 +159,7 @@
 		execve(cmd, argv, envp);
 	} while (errno == EINTR);
 #else
+	argv++;
 	execve(cmd, argv, envp);
 #endif
 	if (cmd != path_bshell && errno == ENOEXEC) {
