--- launcher/main.c.orig	2014-04-04 09:12:48.000000000 +0100
+++ launcher/main.c	2014-04-04 09:16:46.000000000 +0100
@@ -173,6 +173,9 @@
 {
 	size_t		n;
 	const char	*home_dir = NULL;
+#ifdef __riscos__
+	home_dir = getenv("Choices$Write");
+#else
 #if USE_PASSWORD_FILE
 	struct passwd	*pwent;
 
@@ -182,6 +185,7 @@
 	else
 		home_dir = pwent->pw_dir;
 #endif
+#endif
 	if (home_dir == NULL)
 		home_dir = getenv("HOME");
 	if (home_dir == NULL)
@@ -193,7 +197,14 @@
 		Sys_Error ("%s (%d): too small bufsize %d. needed %d.",
 				__FILE__, __LINE__, (int)dstsize, (int)n);
 	}
-
+#ifdef __riscos__
+// Need to add '/' to start so unixlib treats file name as a unix file name
+if (home_dir[0] != '/')
+{
+ *dst++ = '/';
+ dstsize--;
+}
+#endif
 	snprintf (dst, dstsize, "%s/%s", home_dir, AOT_USERDIR);
 	return 0;
 }
