--- Makefile.orig	2009-12-19 07:13:53.000000000 -0800
+++ Makefile	2009-12-19 07:14:08.000000000 -0800
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
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile -lmikmod
 NOSOUNDFLAG=$(SOUND)SOUND
 JOYFLAG=$(JOY)JOYSTICK
 EMBEDDEDFLAG=NOT_EMBEDDED
@@ -31,7 +32,7 @@
 
 CFLAGS=-Wall $(SDL_CFLAGS) \
 	-DDATA_PREFIX=\"$(DATA_PREFIX)\" -D$(NOSOUNDFLAG) -D$(JOYFLAG) \
-	-D$(EMBEDDEDFLAG) $(XTRA_FLAGS)
+	-D$(EMBEDDEDFLAG) $(XTRA_FLAGS) -O2
 SDL_LIB=$(MIXER) $(SDL_LDFLAGS)
 CQML_LIB=-L./cqml -lcqml -L/opt/Qtopia/lib -lqpe -lqte
 #CQML_LIB=-L/opt/Qtopia/lib -lqpe -lqte
@@ -68,7 +69,7 @@
 
 
 gemdropx:	gemdropx.o
-	$(CXX) $(CFLAGS) -o gemdropx gemdropx.o $(GFX_LIB) -lm
+	$(CXX) $(CFLAGS) -o gemdropx gemdropx.o $(GFX_LIB) -lm -static
 
 
 gemdropx.o:	gemdropx.c
