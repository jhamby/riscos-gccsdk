--- src/scicore/tools.c.orig	2004-04-08 01:17:28.000000000 +0100
+++ src/scicore/tools.c	2005-07-08 11:05:23.000000000 +0100
@@ -547,7 +547,7 @@
 
 	return (char *)_path_buf;
 
-#elif defined(__unix__) || !defined(X_DISPLAY_MISSING) || defined (__BEOS__) || defined(MACOSX)
+#elif defined(__unix__) || !defined(X_DISPLAY_MISSING) || defined (__BEOS__) || defined(MACOSX) || defined(__riscos__)
 	return getenv("HOME");
 #elif defined (_DREAMCAST)
 	return "/ram";
