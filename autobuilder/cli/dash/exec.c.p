--- src/exec.c.orig	2014-09-28 21:19:32.000000000 +1300
+++ src/exec.c	2021-01-20 22:40:08.661627099 +1300
@@ -159,6 +159,7 @@
 		execve(cmd, argv, envp);
 	} while (errno == EINTR);
 #else
+	argv++;
 	execve(cmd, argv, envp);
 #endif
 	if (cmd != path_bshell && errno == ENOEXEC) {
@@ -193,7 +194,7 @@
 	if (*path == NULL)
 		return NULL;
 	start = *path;
-	for (p = start ; *p && *p != ':' && *p != '%' ; p++);
+	for (p = start ; *p && *p != ';' && *p != '%' ; p++);
 	len = p - start + strlen(name) + 2;	/* "2" is for '/' and '\0' */
 	while (stackblocksize() < len)
 		growstackblock();
@@ -207,9 +208,9 @@
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
@@ -530,8 +531,8 @@
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
@@ -539,7 +540,7 @@
 			break;
 		if (*new == '%' && bltin < 0 && prefix(new + 1, "builtin"))
 			bltin = idx;
-		if (*new == ':') {
+		if (*new == ';') {
 			idx++;
 		}
 		new++, old++;
