--- src/screen.c.orig	2009-11-13 13:32:39.426464000 +0000
+++ src/screen.c	2009-11-13 13:34:32.862464000 +0000
@@ -39,7 +39,7 @@
 
 // Handle BMP images.
 #define SPRITE_NUM 7
-#define SHARE_LOC "/usr/share/games/noiz2sa/"
+#define SHARE_LOC "/<Noiz2sa$Dir>/data/"
 
 static SDL_Surface *sprite[SPRITE_NUM];
 static char *spriteFile[SPRITE_NUM] = {
