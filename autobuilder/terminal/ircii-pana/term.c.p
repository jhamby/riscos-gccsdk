--- source/term.c        2003-04-11 01:09:07.000000000 +0000
+++ source/term.c      2004-06-20 09:30:48.000000000 +0000
@@ -92,7 +92,7 @@
 #endif

 extern  char    *getenv();
-extern char    *tparm();
+//extern       char    *tparm();

 /*
  * The old code assumed termcap. termcap is almost always present, but on

