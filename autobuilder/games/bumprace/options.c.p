--- src/options.c.orig	2009-06-21 17:39:18.000000000 +0100
+++ src/options.c	2012-10-23 09:09:00.034731900 +0100
@@ -10,11 +10,15 @@
 {
     char filename[200];
     FILE *file;
-    
+
+#ifdef __riscos__
+	sprintf(filename, "<Choices$Write>/bumprace");
+#else
     if ((char *)getenv("HOME") != NULL)
 		snprintf( filename, sizeof(filename), "%s/.bumprace", getenv("HOME"));
     else
 		sprintf( filename, "bumprace.dat");
+#endif
 
     file = fopen(filename, mode);
     
