--- src/Makefile.orig	2014-02-28 11:20:42.000000000 +0000
+++ src/Makefile	2014-02-28 11:50:08.000000000 +0000
@@ -9,7 +9,7 @@
 
 #OPTIMIZATION=-g -pg -fprofile-arcs
 #OPTIMIZATION=-O3
-OPTIMIZATION=-g -O2
+OPTIMIZATION=-static -g -O2
 #OPTIMIZATION=
 
 # SDL library
@@ -20,7 +20,7 @@
 GAME_FLAGS=-D_GNU_SOURCE -Wall -Winline -finline-functions $(SDL_FLAGS) $(OPTIMIZATION)
 GAME_FLAGS+=`dpkg-buildflags --get CFLAGS`
 GAME_FLAGS+=`dpkg-buildflags --get CXXFLAGS`
-GAME_LIBS=-lSDL_mixer $(SDL_LIBS) $(OPTIMIZATION)
+GAME_LIBS=-lSDL_mixer $(SDL_LIBS) $(OPTIMIZATION) -lvorbisidec -logg -lmikmod
 GAME_LIBS+=`dpkg-buildflags --get LDFLAGS`
 
 # all objectfiles
