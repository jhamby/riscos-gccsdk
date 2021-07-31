--- src/SDL_extras.c.orig	2007-12-14 13:11:46.330000000 +0000
+++ src/SDL_extras.c	2007-12-14 13:15:01.470000000 +0000
@@ -63,7 +63,7 @@
   /* now means we are using Debian, so grab from Debian installation loc: */
   if (!f)
   { 
-    sprintf(fontfile, "/usr/share/fonts/truetype/ttf-sil-andika/AndikaDesRevG.ttf");
+    sprintf(fontfile, "/UnixFont:truetype/ttf-sil-andika/AndikaDesRevG.ttf");
     f = TTF_OpenFont(fontfile, font_size);
   }
 
