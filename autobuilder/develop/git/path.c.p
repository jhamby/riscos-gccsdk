--- path.c.orig	2015-11-15 13:36:10.074581000 +0100
+++ path.c	2015-11-15 13:43:57.526581000 +0100
@@ -927,8 +927,11 @@
 	config_home = getenv("XDG_CONFIG_HOME");
 	if (config_home && *config_home)
 		return mkpathdup("%s/git/%s", config_home, filename);
-
+#ifndef __riscos
 	home = getenv("HOME");
+#else
+	home = "/<Choices$Write>";
+#endif
 	if (home)
 		return mkpathdup("%s/.config/git/%s", home, filename);
 	return NULL;
