--- circuslinux.c.orig	2007-01-11 14:41:23.718750000 +0000
+++ circuslinux.c	2007-01-11 14:43:10.562500000 +0000
@@ -3527,12 +3527,12 @@
 {
   char * filename;
   FILE * fi;
-#ifdef LINUX
+#if defined(LINUX) || defined(__riscos__) 
   char * home;
 #endif
   
   
-#ifdef LINUX
+#if defined(LINUX) || defined(__riscos__)
   /* Get home directory (from $HOME variable)... if we can't determine it,
      use the current directory ("."): */
 
