--- Makefile.orig	2015-12-17 17:11:56.337283978 +0000
+++ Makefile	2015-12-17 17:18:38.888599715 +0000
@@ -14,7 +14,8 @@
 PREFIX=/usr/local
 MAN_PREFIX=$(PREFIX)
 BIN_PREFIX=$(PREFIX)/bin
-DATA_PREFIX=$(PREFIX)/share/defendguin/
+#DATA_PREFIX=$(PREFIX)/share/defendguin/
+DATA_PREFIX=/\<Defenguin\$$Dir\>/data/
 JOY=YES
 
 
@@ -22,10 +23,10 @@
 
 SDL_CFLAGS := $(shell sdl-config --cflags)
 SDL_LDFLAGS := $(shell sdl-config --libs) -L/usr/X11R6/lib
-MIXER=-lSDL_mixer
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile -lmikmod
 NOSOUNDFLAG=__SOUND
 CFLAGS=$(DEB_CFLAGS) $(SDL_CFLAGS) -DDATA_PREFIX=\"$(DATA_PREFIX)\" -D$(NOSOUNDFLAG) \
-	-DJOY_$(JOY)
+	-DJOY_$(JOY) -O2 -static
 SDL_LIB=$(SDL_LDFLAGS) $(MIXER)
 
 
