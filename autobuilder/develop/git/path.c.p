--- path.c.orig	2014-12-18 18:42:18.000000000 +0000
+++ path.c	2015-06-05 12:31:22.273995793 +0100
@@ -133,8 +133,13 @@
 void home_config_paths(char **global, char **xdg, char *file)
 {
 	char *xdg_home = getenv("XDG_CONFIG_HOME");
+#ifndef __riscos__	
 	char *home = getenv("HOME");
+#endif	
 	char *to_free = NULL;
+#ifdef __riscos__
+    char *home = "/<Choices$Write>";
+#endif	
 
 	if (!home) {
 		if (global)
