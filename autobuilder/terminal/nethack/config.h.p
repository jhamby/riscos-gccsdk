--- include/config.h.orig	2009-08-27 10:41:18.000000000 -0700
+++ include/config.h	2009-08-27 10:41:26.000000000 -0700
@@ -356,7 +356,7 @@
 
 #ifdef TTY_GRAPHICS
 # define MENU_COLOR
-# define MENU_COLOR_REGEX
+/* # define MENU_COLOR_REGEX*/
 /* if MENU_COLOR_REGEX is defined, use regular expressions (GNU regex.h)
  * otherwise use pmatch() to match menu color lines.
  * pmatch() provides basic globbing: '*' and '?' wildcards.
