--- makefile.orig	2012-01-03 17:58:43.000000000 +0000
+++ makefile	2021-03-06 12:30:38.799461696 +0000
@@ -32,7 +32,7 @@
 ifeq ($(OS),Windows_NT)
 DATA_DIR =
 else
-DATA_DIR = $(PREFIX)/share/games/edgar/
+DATA_DIR = "/<Edgar\$$Dir>/data/"
 endif
 
 ifeq ($(DEV),1)
@@ -46,7 +46,7 @@
 ifeq ($(OS),Windows_NT)
 LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lz -lintl -lm
 else
-LFLAGS = `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lz -lm
+LFLAGS = -static `sdl-config --libs` -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -llzma -lSDL_net -lz -lbz2 -lintl -lpng -ljpeg -lfreetype -lvorbisidec -lvorbisfile -logg -lmikmod -ltiff -lFLAC -lwebp
 endif
 
 TILE_OBJS  = tile_creator.o save_png.o
