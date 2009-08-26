--- src/SDL_extras.c.orig	2009-08-26 13:56:49.000000000 -0700
+++ src/SDL_extras.c	2009-08-26 13:57:14.000000000 -0700
@@ -987,6 +987,8 @@
 /* font in memory once loaded until cleanup.                  */
 static TTF_Font* get_font(int size)
 {
+  static char prev_font_name[FONT_NAME_LENGTH];
+
   if (size < 0)
   {
     fprintf(stderr, "Error - requested font size %d is negative\n", size);
