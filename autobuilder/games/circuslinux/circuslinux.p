--- circuslinux.c.orig	2012-10-11 11:24:55.143664900 +0100
+++ circuslinux.c	2012-10-11 12:53:33.709862300 +0100
@@ -3526,12 +3526,12 @@
 {
   char * filename;
   FILE * fi;
-#ifdef LINUX
+#if defined(LINUX)
   char * home;
 #endif
   
   
-#ifdef LINUX
+#if defined(LINUX)
   /* Get home directory (from $HOME variable)... if we can't determine it,
      use the current directory ("."): */
 
@@ -3557,8 +3557,12 @@
   strcat(filename, "/.circuslinux");
 #endif
 #else
+#ifdef __riscos__
+  filename = "<Choices$Write>/circuslinux";
+#else
   filename = "circuslinux.dat";
 #endif
+#endif
   
   
   /* Try opening the file: */
