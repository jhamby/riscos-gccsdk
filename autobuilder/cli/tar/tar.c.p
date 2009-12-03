--- src/tar.c.orig	2009-12-03 11:00:06.000000000 -0800
+++ src/tar.c	2009-12-03 11:01:00.000000000 -0800
@@ -2433,7 +2433,7 @@
   set_start_time ();
   program_name = argv[0];
 
-  setlocale (LC_ALL, "");
+  //setlocale (LC_ALL, "");
   bindtextdomain (PACKAGE, LOCALEDIR);
   textdomain (PACKAGE);
 
