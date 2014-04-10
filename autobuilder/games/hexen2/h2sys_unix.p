--- engine/hexen2/sys_unix.c.orig	2014-04-08 12:21:28.000000000 +0100
+++ engine/hexen2/sys_unix.c	2014-04-08 12:23:04.000000000 +0100
@@ -528,6 +528,9 @@
 {
 	size_t		n;
 	const char	*home_dir = NULL;
+#ifdef __riscos__
+	home_dir = getenv("Choices$Write");
+#else
 #if USE_PASSWORD_FILE
 	struct passwd	*pwent;
 
@@ -537,6 +540,7 @@
 	else
 		home_dir = pwent->pw_dir;
 #endif
+#endif
 	if (home_dir == NULL)
 		home_dir = getenv("HOME");
 	if (home_dir == NULL)
@@ -553,6 +557,15 @@
 					__thisfunc__, (int)dstsize, (int)n);
 	}
 
+#ifdef __riscos__
+// Need to add '/' to start so unixlib treats file name as a unix file name
+	if (home_dir[0] != '/')
+	{
+		*dst++ = '/';
+		dstsize--;
+	}
+#endif
+
 	q_snprintf (dst, dstsize, "%s/%s", home_dir, AOT_USERDIR);
 	return 0;
 }
