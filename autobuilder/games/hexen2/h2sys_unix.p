--- engine/hexen2/sys_unix.c.orig	2016-03-12 13:59:35.774044838 +0000
+++ engine/hexen2/sys_unix.c	2016-03-12 14:04:40.986045150 +0000
@@ -543,6 +543,9 @@
 {
 	size_t		n;
 	const char	*home_dir = NULL;
+#ifdef __riscos__
+	home_dir = getenv("Choices$Write");
+#else
 	struct passwd	*pwent;
 
 	pwent = getpwuid(getuid());
@@ -551,6 +554,7 @@
 	else	home_dir = pwent->pw_dir;
 	if (home_dir == NULL)
 		home_dir = getenv("HOME");
+#endif
 	if (home_dir == NULL)
 		return 1;
 
@@ -565,6 +569,15 @@
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
