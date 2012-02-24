--- mono/mini/main.c.orig	2011-12-19 21:10:25.000000000 +0000
+++ mono/mini/main.c	2012-02-24 17:50:36.000000000 +0000
@@ -12,7 +12,7 @@
 static int
 mono_main_with_options (int argc, char *argv [])
 {
-	const char *env_options = getenv ("MONO_ENV_OPTIONS");
+	const char *env_options = getenv ("Mono$EnvOptions");
 	if (env_options != NULL){
 		GPtrArray *array = g_ptr_array_new ();
 		GString *buffer = g_string_new ("");
