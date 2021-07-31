--- unix/i_linux.c.orig	2014-04-04 09:44:11.000000000 +0100
+++ unix/i_linux.c	2014-04-04 09:45:42.000000000 +0100
@@ -331,13 +331,23 @@
 #if !defined(_NO_USERDIRS)
 	int rc, sz;
 	char *base;
+#ifdef __riscos__
+	char *homedir = getenv("Choices$Write");
+	if (homedir == NULL) homedir = getenv("HOME");
+#else
 	char *homedir = getenv("HOME");
+#endif
 	if (homedir == NULL)
 		I_Error ("Unable to determine user home directory");
 	/* make sure that basePath has a trailing slash */
 	sz = strlen(homedir) + strlen(H_USERDIR) + 3;
 	base = (char *) malloc(sz * sizeof(char));
+#ifdef __riscos__
+// Ensure unixlib treats paths a unix format path
+	snprintf(base, sz, "/%s/%s/", homedir, H_USERDIR);
+#else
 	snprintf(base, sz, "%s/%s/", homedir, H_USERDIR);
+#endif
 	basePath = base;
 	rc = mkdir(base, S_IRWXU|S_IRWXG|S_IRWXO);
 	if (rc != 0 && errno != EEXIST)
