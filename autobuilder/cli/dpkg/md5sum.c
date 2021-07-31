--- utils/md5sum.c	2002-08-23 05:22:03.000000000 +0100
+++ ../dpkg-1.10.9-ro/utils/md5sum.c	2003-03-12 20:22:36.000000000 +0000
@@ -57,6 +57,10 @@
 #endif
 #endif
 
+#ifdef __riscos__
+#include <unixlib/features.h>
+#endif
+
 const char printforhelp[]= N_("Type md5sum --help for help.");
 const char thisname[]= MD5SUM;
 
@@ -92,6 +96,10 @@
 	FILE *fp = NULL;
 	unsigned char *digest = NULL;
 
+#ifdef __riscos__
+       __feature_imagefs_is_file = 1;
+#endif
+
 	setlocale(LC_ALL, "");
 	bindtextdomain(PACKAGE, LOCALEDIR);
 	textdomain(PACKAGE);
