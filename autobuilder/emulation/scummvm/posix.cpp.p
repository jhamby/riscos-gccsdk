--- backends/platform/sdl/posix/posix.cpp.orig	2011-07-03 21:44:51.000000000 +0100
+++ backends/platform/sdl/posix/posix.cpp	2014-09-17 12:11:01.000000000 +0100
@@ -61,7 +61,11 @@
 
 	// On UNIX type systems, by default we store the config file inside
 	// to the HOME directory of the user.
+#ifdef __riscos__
+	const char *home = "/<Choices$Write>";
+#else        
 	const char *home = getenv("HOME");
+#endif
 	if (home != NULL && strlen(home) < MAXPATHLEN)
 		snprintf(configFile, MAXPATHLEN, "%s/%s", home, _baseConfigName.c_str());
 	else
@@ -71,7 +75,11 @@
 }
 
 Common::WriteStream *OSystem_POSIX::createLogFile() {
+#ifdef __riscos__
+	const char *home = "/<Choices$Write>";
+#else        
 	const char *home = getenv("HOME");
+#endif
 	if (home == NULL)
 		return 0;
 
