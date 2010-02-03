--- src/fcint.h.old	2009-11-16 22:46:18.000000000 +0000
+++ src/fcint.h	2010-01-26 12:36:40.678095900 +0000
@@ -54,7 +54,7 @@
 #define FC_CONFIG_PATH "fonts.conf"
 #endif
 
-#ifdef _WIN32
+#if defined(_WIN32) || defined(__riscos__)
 #define FC_SEARCH_PATH_SEPARATOR ';'
 #else
 #define FC_SEARCH_PATH_SEPARATOR ':'
