--- src/fcint.h.orig	2013-10-11 04:10:18.000000000 +0100
+++ src/fcint.h	2014-01-31 17:18:37.937633876 +0000
@@ -62,6 +62,10 @@
 #  define FC_SEARCH_PATH_SEPARATOR ';'
 #  define FC_DIR_SEPARATOR         '\\'
 #  define FC_DIR_SEPARATOR_S       "\\"
+#elif defined(__riscos__)
+#  define FC_SEARCH_PATH_SEPARATOR ';'
+#  define FC_DIR_SEPARATOR         '/'
+#  define FC_DIR_SEPARATOR_S       "/"
 #else
 #  define FC_SEARCH_PATH_SEPARATOR ':'
 #  define FC_DIR_SEPARATOR         '/'
