--- mono/mini/main.c.orig	2013-01-08 18:41:06.000000000 +0000
+++ mono/mini/main.c	2013-01-16 19:41:46.000000000 +0000
@@ -16,7 +16,7 @@
 static int
 mono_main_with_options (int argc, char *argv [])
 {
-	const char *env_options = getenv ("MONO_ENV_OPTIONS");
+	const char *env_options = getenv ("Mono$EnvOptions");
 	if (env_options != NULL){
 		GPtrArray *array = g_ptr_array_new ();
 		GString *buffer = g_string_new ("");
