--- src/exec.c.orig	2023-07-30 13:52:00.608800927 -0700
+++ src/exec.c	2023-07-30 13:55:11.400105584 -0700
@@ -189,6 +189,7 @@
 		execve(cmd, argv, envp);
 	} while (errno == EINTR);
 #else
+	argv++;
 	execve(cmd, argv, envp);
 #endif
 	if (cmd != path_bshell && errno == ENOEXEC) {
@@ -240,7 +241,7 @@
 
 int padvance_magic(const char **path, const char *name, int magic)
 {
-	const char *term = "%:";
+	const char *term = "%;";
 	const char *lpathopt;
 	const char *p;
 	char *q;
@@ -257,14 +258,14 @@
 	if (*start == '%' && (p = legal_pathopt(start + 1, term, magic))) {
 		lpathopt = start + 1;
 		start = p;
-		term = ":";
+		term = ";";
 	}
 
 	len = strcspn(start, term);
 	p = start + len;
 
 	if (*p == '%') {
-		size_t extra = strchrnul(p, ':') - p;
+		size_t extra = strchrnul(p, ';') - p;
 
 		if (legal_pathopt(p + 1, term, magic))
 			lpathopt = p + 1;
@@ -275,7 +276,7 @@
 	}
 
 	pathopt = lpathopt;
-	*path = *p == ':' ? p + 1 : NULL;
+	*path = *p == ';' ? p + 1 : NULL;
 
 	/* "2" is for '/' and '\0' */
 	qlen = len + strlen(name) + 2;
@@ -611,7 +612,7 @@
 			bltin = idx;
 			break;
 		}
-		new = strchr(new, ':');
+		new = strchr(new, ';');
 		if (!new)
 			break;
 		idx++;
