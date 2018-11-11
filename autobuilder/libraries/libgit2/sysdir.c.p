--- src/sysdir.c.orig	2018-11-11 14:02:44.021320302 +0000
+++ src/sysdir.c	2018-11-11 14:02:56.717063528 +0000
@@ -47,7 +47,9 @@
 
 	assert(out);
 
+#ifndef GIT_RISCOS
 	if ((buflen = sysconf(_SC_GETPW_R_SIZE_MAX)) == -1)
+#endif
 		buflen = 1024;
 
 	do {
@@ -79,6 +81,28 @@
 {
 #ifdef GIT_WIN32
 	return git_win32__find_global_dirs(out);
+#elif defined(GIT_RISCOS)
+	int error;
+	uid_t uid, euid;
+
+	uid = getuid();
+	euid = geteuid();
+
+	/*
+	 * In case we are running setuid, use the configuration
+	 * of the effective user.
+	 */
+	if (uid == euid)
+	    error = git__getenv(out, "Choices$Write");
+	else
+	    error = get_passwd_home(out, euid);
+
+	if (error == GIT_ENOTFOUND) {
+		giterr_clear();
+		error = 0;
+	}
+
+	return error;
 #else
 	int error;
 	uid_t uid, euid;
