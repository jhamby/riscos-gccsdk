--- vid_sdl.c.orig	2012-04-14 05:29:10.000000000 +0100
+++ vid_sdl.c	2012-04-14 05:29:22.000000000 +0100
@@ -56,7 +56,7 @@
     Uint32 flags;
 
     // Load the SDL library
-    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_CDROM) < 0)
+    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
         Sys_Error("VID: Couldn't load SDL: %s", SDL_GetError());
 
     // Set up display mode (width and height)
