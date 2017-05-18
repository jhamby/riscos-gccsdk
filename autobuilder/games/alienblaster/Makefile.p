--- src/Makefile.orig	2017-05-16 13:16:43.172039000 +0100
+++ src/Makefile	2017-05-16 13:20:22.948039000 +0100
@@ -9,7 +9,7 @@
 
 #OPTIMIZATION=-g -pg -fprofile-arcs
 #OPTIMIZATION=-O3
-OPTIMIZATION=-g -O2
+OPTIMIZATION=-static -g -O2
 #OPTIMIZATION=
 
 # SDL library
@@ -18,10 +18,10 @@
 
 # game flags
 GAME_FLAGS=-D_GNU_SOURCE -Wall -Winline -finline-functions $(SDL_FLAGS) $(OPTIMIZATION)
-GAME_FLAGS+=`dpkg-buildflags --get CFLAGS`
-GAME_FLAGS+=`dpkg-buildflags --get CXXFLAGS`
+#GAME_FLAGS+=`dpkg-buildflags --get CFLAGS`
+#GAME_FLAGS+=`dpkg-buildflags --get CXXFLAGS`
-GAME_LIBS=-lSDL_mixer $(SDL_LIBS) $(OPTIMIZATION)
+GAME_LIBS=-lSDL_mixer $(SDL_LIBS) $(OPTIMIZATION) -lvorbisidec -lvorbisfile -logg -lmikmod
-GAME_LIBS+=`dpkg-buildflags --get LDFLAGS`
+#GAME_LIBS+=`dpkg-buildflags --get LDFLAGS`
 
 # all objectfiles
 OBJECT_FILES=main.o surfaceDB.o soundDB.o options.o geometry.o video.o game.o \
