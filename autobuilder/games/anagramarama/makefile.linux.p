--- makefile.linux.orig	2010-08-20 20:08:52.000000000 +0100
+++ makefile.linux	2010-08-20 19:51:16.000000000 +0100
@@ -1,5 +1,5 @@
-LFLAGS=-Wall -funroll-loops -fomit-frame-pointer -pipe -O9
-CFLAGS=-Wall `sdl-config --cflags --libs` -funroll-loops -fomit-frame-pointer -pipe -O9 -lSDL_mixer
+LFLAGS=-Wall -funroll-loops -fomit-frame-pointer -pipe -O3
+CFLAGS=-Wall -static `sdl-config --cflags --libs` -funroll-loops -fomit-frame-pointer -pipe -O3 -lSDL_mixer -lvorbisidec
 CC=gcc
 
 C_FILES=src/dlb.c src/linked.c src/sprite.c src/ag.c
