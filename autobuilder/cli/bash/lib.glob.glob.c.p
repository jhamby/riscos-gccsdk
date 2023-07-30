--- lib/glob/glob.c.orig	2020-10-30 11:49:00.000000000 -0700
+++ lib/glob/glob.c	2023-07-30 15:29:26.686256732 -0700
@@ -119,10 +119,10 @@
 #if HANDLE_MULTIBYTE
 void wcdequote_pathname PARAMS((wchar_t *));
 static void wdequote_pathname PARAMS((char *));
+static void dequote_pathname PARAMS((char *));
 #else
 #  define dequote_pathname udequote_pathname
 #endif
-static void dequote_pathname PARAMS((char *));
 static int glob_testdir PARAMS((char *, int));
 static char **glob_dir_to_array PARAMS((char *, char **, int));
 
