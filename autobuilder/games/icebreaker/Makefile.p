--- Makefile.orig	2006-12-22 14:32:28.859375000 +0000
+++ Makefile	2006-12-22 14:34:27.265625000 +0000
@@ -8,7 +8,8 @@
 endif
 
 ifndef datadir
-  datadir=$(prefix)/share
+  #datadir=$(prefix)/share
+  datadir=\<IceBreaker\$$Dir\>/data
 endif
 
 ifndef mandir
@@ -16,21 +17,22 @@
 endif
 
 ifndef highscoredir
-  highscoredir=/var/local/lib/games
+  #highscoredir=/var/local/lib/games
   #highscoredir=/var/lib/games
+  highscoredir=\<IceBreaker$$Dir\>/data
 endif
 
 ifndef OPTIMIZE
   OPTIMIZE=-O3
 endif
 
-CFLAGS=-Wall $(OPTIMIZE) $(SDL_CFLAGS) -DDATAPREFIX=\"$(datadir)/icebreaker\" -DHISCOREPREFIX=\"$(highscoredir)\"
+CFLAGS=-Wall $(OPTIMIZE) $(SDL_CFLAGS) -DDATAPREFIX=\"$(datadir)\" -DHISCOREPREFIX=\"$(highscoredir)\"
 
 SRC=icebreaker.c cursor.c grid.c laundry.c line.c penguin.c sound.c \
     level.c intro.c text.c status.c transition.c hiscore.c dialog.c \
     options.c fullscreen.c
 
-SDL_MIXER=-lSDL_mixer
+SDL_MIXER=-lSDL_mixer -logg -lvorbisfile -lvorbis
 SDL_LIB=$(SDL_LDFLAGS) $(SDL_MIXER) 
 SDL_CFLAGS := $(shell $(SDLCONFIG) --cflags)
 SDL_LDFLAGS := $(shell $(SDLCONFIG) --libs)
