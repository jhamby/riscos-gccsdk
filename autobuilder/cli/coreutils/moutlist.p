--- lib/mountlist.c~	2004-01-26 08:58:12.000000000 +0000
+++ lib/mountlist.c	2005-03-06 21:59:40.000000000 +0000
@@ -99,6 +99,7 @@
 # include <dirent.h>
 #endif
 
+#undef MOUNTED_FREAD
 #ifdef MOUNTED_FREAD		/* SVR2.  */
 # include <mnttab.h>
 #endif
