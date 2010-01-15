--- Makefile.orig	2009-08-03 08:20:54.000000000 -0700
+++ Makefile	2009-08-03 08:22:25.000000000 -0700
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
+SDL_MIXER=-lSDL_mixer -logg -lvorbisfile -lvorbis -static
 SDL_LIB=$(SDL_LDFLAGS) $(SDL_MIXER) 
 SDL_CFLAGS := $(shell $(SDLCONFIG) --cflags)
 SDL_LDFLAGS := $(shell $(SDLCONFIG) --libs)
