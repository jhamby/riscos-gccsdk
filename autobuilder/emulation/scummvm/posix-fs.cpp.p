--- backends/fs/posix/posix-fs.cpp.orig	2011-07-03 21:44:51.000000000 +0100
+++ backends/fs/posix/posix-fs.cpp	2014-09-17 12:11:12.000000000 +0100
@@ -51,7 +51,11 @@
 
 	// Expand "~/" to the value of the HOME env variable
 	if (p.hasPrefix("~/")) {
+#ifdef __riscos__
+	const char *home = "/<Choices$Write>";
+#else
 		const char *home = getenv("HOME");
+#endif
 		if (home != NULL && strlen(home) < MAXPATHLEN) {
 			_path = home;
 			// Skip over the tilda.  We know that p contains at least
