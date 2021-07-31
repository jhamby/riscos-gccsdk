--- src/common.h.orig	2020-10-08 19:40:07.758528000 +1300
+++ src/common.h	2020-10-08 20:21:40.604645031 +1300
@@ -336,7 +336,9 @@
 GLOBAL const char *volume_label_option;
 
 /* Other global variables.  */
-
+# ifndef ZSTD_PROGRAM
+# define ZSTD_PROGRAM "zstd"
+#endif
 /* File descriptor for archive file.  */
 GLOBAL int archive;
 
