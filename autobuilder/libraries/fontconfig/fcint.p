--- src/fcint.h.orig	2018-07-19 04:14:39.000000000 +0100
+++ src/fcint.h	2021-10-30 11:52:13.836359663 +0100
@@ -64,6 +64,10 @@
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
