--- Makefile.orig	2007-08-03 11:37:51.870732000 +0100
+++ Makefile	2007-08-03 11:40:45.178732000 +0100
@@ -14,7 +14,8 @@
 PREFIX=/usr/local
 MAN_PREFIX=$(PREFIX)
 BIN_PREFIX=$(PREFIX)/bin
-DATA_PREFIX=$(PREFIX)/share/defendguin/
+#DATA_PREFIX=$(PREFIX)/share/defendguin/
+DATA_PREFIX=/\<Defenguin\$$Dir\>/data/
 JOY=YES
 
 
@@ -22,7 +23,7 @@
 
 SDL_CFLAGS := $(shell sdl-config --cflags)
 SDL_LDFLAGS := $(shell sdl-config --libs) -L/usr/X11R6/lib
-MIXER=-lSDL_mixer
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile
 NOSOUNDFLAG=__SOUND
 CFLAGS=-Wall $(SDL_CFLAGS) -DDATA_PREFIX=\"$(DATA_PREFIX)\" -D$(NOSOUNDFLAG) \
 	-DJOY_$(JOY)
