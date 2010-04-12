--- lib/tempname.c.orig	2010-03-27 13:36:25.000000000 +0000
+++ lib/tempname.c	2010-03-27 13:36:48.000000000 +0000
@@ -52,7 +52,7 @@
 #include <string.h>
 
 #include <fcntl.h>
-#include <sys/time.h>
+/* #include <sys/time.h> */
 #include <stdint.h>
 #include <unistd.h>
 
