--- gnulib/lib/mountlist.c.orig	2009-11-23 12:47:47.000000000 -0800
+++ gnulib/lib/mountlist.c	2009-11-23 12:47:54.000000000 -0800
@@ -90,7 +90,7 @@
 #endif
 
 #ifdef MOUNTED_FREAD		/* SVR2.  */
-# include <mnttab.h>
+//# include <mnttab.h>
 #endif
 
 #ifdef MOUNTED_FREAD_FSTYP	/* SVR3.  */
@@ -634,7 +634,11 @@
   }
 #endif /* MOUNTED_GETFSSTAT */
 
-#if defined MOUNTED_FREAD || defined MOUNTED_FREAD_FSTYP /* SVR[23].  */
+#ifdef __riscos__
+
+  return NULL;
+
+#elif defined MOUNTED_FREAD || defined MOUNTED_FREAD_FSTYP /* SVR[23].  */
   {
     struct mnttab mnt;
     char *table = "/etc/mnttab";
