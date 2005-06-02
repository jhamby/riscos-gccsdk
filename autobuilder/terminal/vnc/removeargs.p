--- vncviewer/argsresources.c.orig	2005-06-01 13:24:35.000000000 +0100
+++ vncviewer/argsresources.c	2005-06-01 13:24:40.000000000 +0100
@@ -278,22 +278,6 @@
 
 
 /*
- * removeArgs() is used to remove some of command line arguments.
- */
-
-void
-removeArgs(int *argc, char** argv, int idx, int nargs)
-{
-  int i;
-  if ((idx+nargs) > *argc) return;
-  for (i = idx+nargs; i < *argc; i++) {
-    argv[i-nargs] = argv[i];
-  }
-  *argc -= nargs;
-}
-
-
-/*
  * usage() prints out the usage message.
  */
 
