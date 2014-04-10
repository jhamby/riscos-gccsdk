--- makefile.orig	2011-05-16 21:00:04.000000000 +0100
+++ makefile	2011-05-21 13:02:54.798169461 +0100
@@ -48,7 +48,7 @@
 ifeq ($(OS),Windows_NT)
 LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lz -llibintl -lm
 else
-LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lz
+LFLAGS = -static `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lz -lbz2 -lintl -lpng -ljpeg -lfreetype -lvorbisidec -lvorbisfile -logg -lmikmod -ltiff
 endif
 
 TILE_OBJS  = tile_creator.o save_png.o
