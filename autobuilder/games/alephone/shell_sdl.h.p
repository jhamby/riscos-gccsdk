--- Source_Files/shell_sdl.h.orig	2005-07-17 21:21:18.000000000 +0100
+++ Source_Files/shell_sdl.h	2008-09-04 11:26:54.710000000 +0100
@@ -354,11 +354,19 @@
 	local_data_dir.CreateDirectory();
 	local_data_dir += login;
 
+#elif defined(__riscos__)
+
+	default_data_dir = "<Alephone$Dir>";
+	const char *home = getenv("HOME");
+	if (home)
+		local_data_dir = home;
+	local_data_dir += ".alephone";
+
 #else
 #error Data file paths must be set for this platform.
 #endif
 
-#if defined(__WIN32__)
+#if defined(__WIN32__) || defined(__riscos__)
 #define LIST_SEP ';'
 #else
 #define LIST_SEP ':'
