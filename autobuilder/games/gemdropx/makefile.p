--- Makefile.orig	2009-08-19 14:03:01.000000000 -0700
+++ Makefile	2009-08-19 14:03:43.000000000 -0700
@@ -16,14 +16,15 @@
 installbin = install -g root -o root -m 755
 installdat = install -g root -o root -m 644
 
-DATA_PREFIX=/usr/local/share/gemdropx
+#DATA_PREFIX=/usr/local/share/gemdropx
+DATA_PREFIX=/\<GemDropX\$$Dir\>/data/
 SDL_CFLAGS := $(shell sdl-config --cflags)
 SDL_LDFLAGS := $(shell sdl-config --libs)
-CC=gcc
-CXX=gcc
+#CC=gcc
+#CXX=gcc
 
 
-MIXER=-lSDL_mixer
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile
 NOSOUNDFLAG=$(SOUND)SOUND
 JOYFLAG=$(JOY)JOYSTICK
 EMBEDDEDFLAG=NOT_EMBEDDED
@@ -68,7 +69,7 @@
 
 
 gemdropx:	gemdropx.o
-	$(CXX) $(CFLAGS) -o gemdropx gemdropx.o $(GFX_LIB) -lm
+	$(CXX) $(CFLAGS) -o gemdropx gemdropx.o $(GFX_LIB) -lm -static
 
 
 gemdropx.o:	gemdropx.c
