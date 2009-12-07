--- Source_Files/shell.cpp.orig	2009-12-07 15:33:00.000000000 -0800
+++ Source_Files/shell.cpp	2009-12-07 15:33:52.000000000 -0800
@@ -346,13 +346,21 @@
 	local_data_dir.CreateDirectory();
 	local_data_dir += login;
 
++#elif defined(__riscos__)
+
+        default_data_dir = "<Alephone$Dir>";
+        const char *home = getenv("HOME");
+        if (home)
+               local_data_dir = home;
+        local_data_dir += ".alephone";
+
 #else
 	default_data_dir = "";
 	local_data_dir = "";
 //#error Data file paths must be set for this platform.
 #endif
 
-#if defined(__WIN32__)
+#if defined(__WIN32__) || defined(__riscos__)
 #define LIST_SEP ';'
 #else
 #define LIST_SEP ':'
