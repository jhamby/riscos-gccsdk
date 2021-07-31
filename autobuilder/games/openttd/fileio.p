--- src/fileio.cpp.orig	2009-11-03 20:14:08.000000000 -0800
+++ src/fileio.cpp	2009-11-03 20:26:37.000000000 -0800
@@ -831,6 +831,8 @@
 	char tmp[MAX_PATH];
 #if defined(__MORPHOS__) || defined(__AMIGA__) || defined(DOS) || defined(OS2) || !defined(WITH_PERSONAL_DIR)
 	_searchpaths[SP_PERSONAL_DIR] = NULL;
+#elif defined(__riscos__)
+        _searchpaths[SP_PERSONAL_DIR] = PERSONAL_DIR;
 #else
 	const char *homedir = getenv("HOME");
 
@@ -853,7 +855,7 @@
 	_searchpaths[SP_SHARED_DIR] = NULL;
 #endif
 
-#if defined(__MORPHOS__) || defined(__AMIGA__)
+#if defined(__MORPHOS__) || defined(__AMIGA__) || defined(__riscos__)
 	_searchpaths[SP_WORKING_DIR] = NULL;
 #else
 	if (getcwd(tmp, MAX_PATH) == NULL) *tmp = '\0';
@@ -867,7 +869,7 @@
 	AppendPathSeparator(tmp, MAX_PATH);
 	_searchpaths[SP_BINARY_DIR] = strdup(tmp);
 
-#if defined(__MORPHOS__) || defined(__AMIGA__) || defined(DOS) || defined(OS2)
+#if defined(__MORPHOS__) || defined(__AMIGA__) || defined(DOS) || defined(OS2) || defined(__riscos__)
 	_searchpaths[SP_INSTALLATION_DIR] = NULL;
 #else
 	snprintf(tmp, MAX_PATH, "%s", GLOBAL_DATA_DIR);
