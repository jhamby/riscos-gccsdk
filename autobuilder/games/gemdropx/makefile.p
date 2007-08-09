--- Makefile.orig	2007-08-06 12:53:24.341518100 +0100
+++ Makefile	2007-08-06 12:54:34.659518100 +0100
@@ -16,14 +16,15 @@
 installbin = install -g root -o root -m 755
 installdat = install -g root -o root -m 644
 
-DATA_PREFIX=/usr/local/share/gemdropx
+#DATA_PREFIX=/usr/local/share/gemdropx
+DATA_PREFIX=/\<GemDropX\$$Dir\>/data/
 SDL_CFLAGS := $(shell sdl-config --cflags)
 SDL_LDFLAGS := $(shell sdl-config --libs)
 CC=gcc
 CXX=gcc
 
 
-MIXER=-lSDL_mixer
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile
 NOSOUNDFLAG=$(SOUND)SOUND
 JOYFLAG=$(JOY)JOYSTICK
 EMBEDDEDFLAG=NOT_EMBEDDED
