--- utility/shared.c.orig	2009-03-24 16:22:44.000000000 -0700
+++ utility/shared.c	2009-11-05 14:39:44.000000000 -0800
@@ -54,7 +54,7 @@
 #include "shared.h"
 
 #ifndef PATH_SEPARATOR
-#if defined _WIN32 || defined __WIN32__ || defined __EMX__ || defined __DJGPP__
+#if defined _WIN32 || defined __WIN32__ || defined __EMX__ || defined __DJGPP__ || defined __riscos__
   /* Win32, OS/2, DOS */
 # define PATH_SEPARATOR ";"
 #else
@@ -766,6 +766,8 @@
 {
 #ifdef AMIGA
   return "PROGDIR:";
+#elif !defined(riscos)
+  return fc_strdup("/<Choices$Write>/Freeciv/");
 #else
   static bool init = FALSE;
   static char *home_dir = NULL;
