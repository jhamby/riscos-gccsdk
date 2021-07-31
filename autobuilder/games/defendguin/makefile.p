--- Makefile.orig	2021-03-06 15:20:03.102701184 +0000
+++ Makefile	2021-03-06 15:22:30.644632637 +0000
@@ -14,7 +14,7 @@
 PREFIX=$(DESTDIR)/usr
 MAN_PREFIX=$(PREFIX)/share/man/man6
 BIN_PREFIX=$(PREFIX)/games
-DATA_PREFIX=$(PREFIX)/share/games/defendguin/
+DATA_PREFIX=/\<Defenguin\$$Dir\>/data/
 JOY=YES
 
 
@@ -22,10 +22,10 @@
 
 SDL_CFLAGS := $(shell sdl-config --cflags)
 SDL_LDFLAGS := $(shell sdl-config --libs) -L/usr/X11R6/lib
-MIXER=-lSDL_mixer
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile -lmikmod -lFLAC
 NOSOUNDFLAG=__SOUND
 CFLAGS= $(DEB_CFLAGS) $(SDL_CFLAGS) \
-	-DDATA_PREFIX=\"$(DATA_PREFIX)\" -D$(NOSOUNDFLAG) -DJOY_$(JOY)
+	-DDATA_PREFIX=\"$(DATA_PREFIX)\" -D$(NOSOUNDFLAG) -DJOY_$(JOY) -O3 -static
 SDL_LIB=$(SDL_LDFLAGS) $(MIXER)
 
 
