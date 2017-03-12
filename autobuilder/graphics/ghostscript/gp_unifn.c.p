--- base/gp_unifn.c.orig	2012-09-26 16:10:50.000000000 +0100
+++ base/gp_unifn.c	2012-09-26 16:11:16.000000000 +0100
@@ -19,7 +19,7 @@
 #include "gsutil.h"

 /* Define the character used for separating file names in a list. */
-const char gp_file_name_list_separator = ':';
+const char gp_file_name_list_separator = ',';

 /* Define the string to be concatenated with the file mode */
 /* for opening files without end-of-line conversion. */
