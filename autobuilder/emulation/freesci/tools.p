--- src/scicore/tools.c.orig	2008-03-20 18:03:23.000000000 -0700
+++ src/scicore/tools.c	2009-08-27 20:23:17.000000000 -0700
@@ -484,7 +484,7 @@
 	strncat(_path_buf, path, MAX_PATH - 4);
 
 	return _path_buf;
-#elif defined(__unix__) || !defined(X_DISPLAY_MISSING) || defined (__BEOS__) || defined(MACOSX)
+#elif defined(__unix__) || !defined(X_DISPLAY_MISSING) || defined (__BEOS__) || defined(MACOSX) || defined(__riscos__)
 	return getenv("HOME");
 #elif defined(_DREAMCAST)
 	return NULL;
