--- utility/shared.c.orig	2015-08-15 11:15:15.000000000 +0000
+++ utility/shared.c	2021-03-07 09:52:08.833188840 +0000
@@ -59,7 +59,7 @@
 #include "shared.h"
 
 #ifndef PATH_SEPARATOR
-#if defined _WIN32 || defined __WIN32__ || defined __EMX__ || defined __DJGPP__
+#if defined _WIN32 || defined __WIN32__ || defined __EMX__ || defined __DJGPP__ || defined __riscos__
   /* Win32, OS/2, DOS */
 # define PATH_SEPARATOR ";"
 #else
@@ -735,6 +735,8 @@
 {
 #ifdef AMIGA
   return "PROGDIR:";
+#elif !defined(riscos)
+  return fc_strdup("/<Choices$Write>/Freeciv/");
 #else  /* AMIGA */
   static bool init = FALSE;
   static char *home_dir = NULL;
