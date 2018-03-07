--- src/exec.c.orig	2018-02-23 09:59:30.099939830 +1300
+++ src/exec.c	2018-03-02 16:31:24.481557969 +1300
@@ -151,15 +151,15 @@
 STATIC void
 tryexec(char *cmd, char **argv, char **envp)
 {
-	char *const path_bshell = _PATH_BSHELL;
-
+	char *const path_bshell = "$(HOME)/bin/ash";
+TRACE(("tryexec for cmd %s, argv[0] %s, argv[1] %s, envp[0] %s\n", cmd, argv[0],argv[1], envp[0]));
 repeat:
 #ifdef SYSV
 	do {
 		execve(cmd, argv, envp);
 	} while (errno == EINTR);
 #else
-	execve(cmd, argv, envp);
+	execve(cmd, ++argv, envp);
 #endif
 	if (cmd != path_bshell && errno == ENOEXEC) {
 		*argv-- = cmd;
@@ -193,7 +193,7 @@
 	if (*path == NULL)
 		return NULL;
 	start = *path;
-	for (p = start ; *p && *p != ':' && *p != '%' ; p++);
+	for (p = start ; *p && *p != ';' && *p != '%' ; p++);
 	len = p - start + strlen(name) + 2;	/* "2" is for '/' and '\0' */
 	while (stackblocksize() < len)
 		growstackblock();
@@ -207,9 +207,9 @@
 	pathopt = NULL;
 	if (*p == '%') {
 		pathopt = ++p;
-		while (*p && *p != ':')  p++;
+		while (*p && *p != ';')  p++;
 	}
-	if (*p == ':')
+	if (*p == ';')
 		*path = p + 1;
 	else
 		*path = NULL;
