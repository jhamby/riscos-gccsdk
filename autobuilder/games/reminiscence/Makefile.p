--- Makefile.orig	2005-06-24 16:13:36.000000000 +0100
+++ Makefile	2005-06-24 16:13:46.000000000 +0100
@@ -1,5 +1,5 @@
 
-SDL_CFLAGS = `sdl-config --cflags`
+SDL_CFLAGS = `sdl-config --cflags` -I/home/riscos/env/include
 SDL_LIBS = `sdl-config --libs`
 
 DEFINES = -DSYS_LITTLE_ENDIAN
