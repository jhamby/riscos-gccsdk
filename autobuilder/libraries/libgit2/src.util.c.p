--- src/util.c.orig	2018-11-11 13:25:58.778295447 +0000
+++ src/util.c	2018-11-11 13:49:11.109835222 +0000
@@ -836,6 +836,24 @@
 	git__free(wide_value);
 	return error;
 }
+#elif defined(GIT_RISCOS)
+int git__getenv(git_buf *out, const char *name)
+{
+	const char *val = getenv(name);
+	char * fullpath = NULL;
+	int fullpath_len;
+
+	git_buf_clear(out);
+
+	if (!val)
+		return GIT_ENOTFOUND;
+
+	fullpath_len = strlen(val) + 2;
+	fullpath = git__malloc(fullpath_len * sizeof(char));
+	GITERR_CHECK_ALLOC(fullpath);
+	snprintf(fullpath, fullpath_len, "/%s", val);
+	return git_buf_puts(out, fullpath);
+}
 #else
 int git__getenv(git_buf *out, const char *name)
 {
