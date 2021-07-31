--- Makefile.orig	2021-03-06 16:51:43.115289050 +0000
+++ Makefile	2021-03-06 16:54:36.805741245 +0000
@@ -8,7 +8,8 @@
 endif
 
 ifndef datadir
-  datadir=$(prefix)/share
+  #datadir=$(prefix)/share
+  datadir=\<IceBreaker\$$Dir\>/data
 endif
 
 ifndef mandir
@@ -16,7 +17,7 @@
 endif
 
 ifndef highscoredir
-  highscoredir=/var/local/lib/games
+  highscoredir=\<IceBreaker$$Dir\>/data
   #highscoredir=/var/lib/games
 endif
 
@@ -25,13 +26,13 @@
 endif
 
 CFLAGS?=-Wall $(OPTIMIZE)
-CFLAGS+=$(SDL_CFLAGS) -DDATAPREFIX=\"$(datadir)/icebreaker\" -DHISCOREPREFIX=\"$(highscoredir)\"
+CFLAGS+=$(SDL_CFLAGS) -DDATAPREFIX=\"$(datadir)\" -DHISCOREPREFIX=\"$(highscoredir)\"
 
 SRC=icebreaker.c cursor.c grid.c laundry.c line.c penguin.c sound.c \
     level.c intro.c text.c status.c transition.c hiscore.c dialog.c \
     options.c fullscreen.c
 
-SDL_MIXER=-lSDL_mixer
+SDL_MIXER=-lSDL_mixer -logg -lvorbisfile -lvorbis -lmikmod -lFLAC -static
 SDL_LIB=$(SDL_LDFLAGS) $(SDL_MIXER) 
 SDL_CFLAGS := $(shell $(SDLCONFIG) --cflags)
 SDL_LDFLAGS := $(shell $(SDLCONFIG) --libs)
