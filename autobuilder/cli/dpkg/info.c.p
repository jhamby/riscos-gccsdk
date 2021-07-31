--- dpkg-deb/info.c.orig	2010-06-10 23:19:57.688688611 +0100
+++ dpkg-deb/info.c	2010-06-10 23:20:48.018684283 +0100
@@ -47,6 +47,11 @@
 
 #include "dpkg-deb.h"
 
+char *mkdtemp(char *template)
+{
+	return NULL;
+}
+
 static void cu_info_prepare(int argc, void **argv) {
   pid_t c1;
   char *directory;
