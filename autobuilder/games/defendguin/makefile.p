--- Makefile.orig	2009-12-19 07:17:29.000000000 -0800
+++ Makefile	2009-12-19 07:20:21.000000000 -0800
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
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile
 NOSOUNDFLAG=__SOUND
 CFLAGS=-Wall $(SDL_CFLAGS) -DDATA_PREFIX=\"$(DATA_PREFIX)\" -D$(NOSOUNDFLAG) \
-	-DJOY_$(JOY)
+	-DJOY_$(JOY) -O2 -static
 SDL_LIB=$(SDL_LDFLAGS) $(MIXER)
 
 
