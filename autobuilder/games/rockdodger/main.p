--- main.c.orig	2007-08-10 14:56:43.099328000 +0100
+++ main.c	2007-08-10 14:58:09.209328000 +0100
@@ -926,11 +926,16 @@
     // default: ./images
     // second alternative: ROCK_DODGERS_IMAGEDIR
     // final alternative: /usr/share/rockdodger/images
+#ifdef __riscos__
+    data_dir = strdup("<RockD$Dir>/data");
+    if (missing(data_dir))
+#else
 #if NOT_IN_DEBIAN
     /* changed for Debian, only search env var and /usr/share/rockdodger */
     data_dir = strdup("./data");
     if (missing(data_dir))
 #endif
+#endif
     {
 	char *env;
 	env = getenv("ROCK_DODGERS_IMAGEDIR");
