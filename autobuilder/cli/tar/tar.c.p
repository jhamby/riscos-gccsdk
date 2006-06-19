--- src/tar.c	2004-12-19 09:01:07.000000000 +1300
+++ src/tar.c	2006-04-26 02:05:14.113059936 +1200
@@ -1621,7 +1621,6 @@
 {
   set_start_time ();
   program_name = argv[0];
-  setlocale (LC_ALL, "");
   bindtextdomain (PACKAGE, LOCALEDIR);
   textdomain (PACKAGE);

