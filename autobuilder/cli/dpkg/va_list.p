--- lib/dpkg-db.h.orig	2009-07-25 10:38:13.000000000 -0700
+++ lib/dpkg-db.h	2009-07-25 10:38:24.000000000 -0700
@@ -30,6 +30,8 @@
 #include <sys/types.h>
 #include <stdio.h>
 
+#define va_list __gnuc_va_list
+
 struct versionrevision {
   unsigned long epoch;
   const char *version;
