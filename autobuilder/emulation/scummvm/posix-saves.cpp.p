--- backends/saves/posix/posix-saves.cpp.orig	2011-07-03 21:44:51.000000000 +0100
+++ backends/saves/posix/posix-saves.cpp	2014-09-17 12:11:06.000000000 +0100
@@ -51,7 +51,11 @@
 	ConfMan.registerDefault("savepath", "/mtd_wiselink/scummvm savegames");
 #else
 	Common::String savePath;
+#ifdef __riscos__
+	const char *home = "/<Choices$Write>";
+#else        
 	const char *home = getenv("HOME");
+#endif
 	if (home && *home && strlen(home) < MAXPATHLEN) {
 		savePath = home;
 		savePath += "/" DEFAULT_SAVE_PATH;
