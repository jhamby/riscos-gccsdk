--- src/Makefile.orig	2010-08-21 12:19:51.000000000 +0100
+++ src/Makefile	2010-08-19 23:29:59.000000000 +0100
@@ -9,7 +9,7 @@

 #OPTIMIZATION=-g -pg -fprofile-arcs
 #OPTIMIZATION=-O3
-OPTIMIZATION=-g -O2
+OPTIMIZATION=-static -g -O2
 #OPTIMIZATION=

 # SDL library
@@ -18,7 +18,7 @@

 # game flags
 GAME_FLAGS=-D_GNU_SOURCE -Wall -Winline -finline-functions $(SDL_FLAGS) $(OPTIMIZATION)
-GAME_LIBS=-lSDL_mixer $(SDL_LIBS) $(OPTIMIZATION)
+GAME_LIBS=-lSDL_mixer $(SDL_LIBS) $(OPTIMIZATION) -lvorbisidec -logg -lmikmod

 # all objectfiles
 OBJECT_FILES=main.o surfaceDB.o soundDB.o options.o geometry.o video.o game.o \
