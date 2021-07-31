--- Source_Files/shell.cpp.orig	2009-12-18 18:44:28.000000000 -0800
+++ Source_Files/shell.cpp	2009-12-18 18:43:29.000000000 -0800
@@ -346,13 +346,19 @@
 	local_data_dir.CreateDirectory();
 	local_data_dir += login;
 
+#elif defined(__riscos__)
+
+        default_data_dir = "/<AlephOne$Dir>/";
+        local_data_dir = "/<Choices$Write>/AlephOne/";
+        local_data_dir.CreateDirectory();
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
