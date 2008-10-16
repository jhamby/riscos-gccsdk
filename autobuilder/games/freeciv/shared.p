--- utility/shared.c.orig	2008-06-16 08:32:59.000000000 +0100
+++ utility/shared.c	2008-09-11 12:29:21.257353800 +0100
@@ -53,7 +53,7 @@
 #include "shared.h"
 
 #ifndef PATH_SEPARATOR
-#if defined _WIN32 || defined __WIN32__ || defined __EMX__ || defined __DJGPP__
+#if defined _WIN32 || defined __WIN32__ || defined __EMX__ || defined __DJGPP__ || defined __riscos__
   /* Win32, OS/2, DOS */
 # define PATH_SEPARATOR ";"
 #else
