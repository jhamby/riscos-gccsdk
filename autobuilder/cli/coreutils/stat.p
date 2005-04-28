--- src/stat.c~	2004-02-05 13:46:12.000000000 +0000
+++ src/stat.c	2005-03-06 22:04:43.000000000 +0000
@@ -41,6 +41,7 @@
 #  include <nfs/vfs.h>
 # endif
 #endif
+#include <sys/statfs.h>
 
 #include "system.h"
 
@@ -618,7 +619,8 @@
 do_statfs (char const *filename, int terse, char const *format)
 {
   STRUCT_STATVFS statfsbuf;
-  int i = statfs (filename, &statfsbuf);
+  //  int i = statfs (filename, &statfsbuf);
+  int i = -1;
 
   if (i == -1)
     {
