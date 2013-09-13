--- mono/mini/main.c.orig	2013-07-30 02:47:38.000000000 +0100
+++ mono/mini/main.c	2013-08-02 20:24:44.863899125 +0100
@@ -14,7 +14,7 @@
 static int
 mono_main_with_options (int argc, char *argv [])
 {
-	const char *env_options = getenv ("MONO_ENV_OPTIONS");
+	const char *env_options = getenv ("Mono$EnvOptions");
 	if (env_options != NULL){
 		GPtrArray *array = g_ptr_array_new ();
 		GString *buffer = g_string_new ("");
