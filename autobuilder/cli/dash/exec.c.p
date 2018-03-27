--- src/exec.c.orig	2018-02-23 09:59:30.099939830 +1300
+++ src/exec.c	2018-03-25 22:49:31.017869178 +1300
@@ -118,13 +118,16 @@
 		e = errno;
 	} else {
 		e = ENOENT;
+
 		while ((cmdname = padvance(&path, argv[0])) != NULL) {
 			if (--idx < 0 && pathopt == NULL) {
-				tryexec(cmdname, argv, envp);
+                if (access(cmdname, F_OK) == 0) {  
+				tryexec(cmdname, argv, envp); 
 				if (errno != ENOENT && errno != ENOTDIR)
 					e = errno;
-			}
 			stunalloc(cmdname);
+                 }
+             }   
 		}
 	}
 
@@ -151,7 +154,7 @@
 STATIC void
 tryexec(char *cmd, char **argv, char **envp)
 {
-	char *const path_bshell = _PATH_BSHELL;
+	char *const path_bshell = "$HOME/bin/ash";
 
 repeat:
 #ifdef SYSV
@@ -159,6 +162,8 @@
 		execve(cmd, argv, envp);
 	} while (errno == EINTR);
 #else
+/* TRACE(("tryexec for cmd %s, argv[0] %s, argv[1] %s, envp[0] %s\n", cmd, argv[0],argv[1], envp[0])); */
+    argv++;
 	execve(cmd, argv, envp);
 #endif
 	if (cmd != path_bshell && errno == ENOEXEC) {
@@ -189,11 +194,11 @@
 	char *q;
 	const char *start;
 	size_t len;
-
+/* TRACE(("padvance path %s,  name %s,\n", *path, name)); */
 	if (*path == NULL)
 		return NULL;
 	start = *path;
-	for (p = start ; *p && *p != ':' && *p != '%' ; p++);
+	for (p = start ; *p && *p != ';' && *p != '%' ; p++);
 	len = p - start + strlen(name) + 2;	/* "2" is for '/' and '\0' */
 	while (stackblocksize() < len)
 		growstackblock();
@@ -207,9 +212,9 @@
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
@@ -530,8 +535,8 @@
 	for (;;) {
 		if (*old != *new) {
 			firstchange = idx;
-			if ((*old == '\0' && *new == ':')
-			 || (*old == ':' && *new == '\0'))
+			if ((*old == '\0' && *new == ';')
+			 || (*old == ';' && *new == '\0'))
 				firstchange++;
 			old = new;	/* ignore subsequent differences */
 		}
@@ -539,7 +544,7 @@
 			break;
 		if (*new == '%' && bltin < 0 && prefix(new + 1, "builtin"))
 			bltin = idx;
-		if (*new == ':') {
+		if (*new == ';') {
 			idx++;
 		}
 		new++, old++;
