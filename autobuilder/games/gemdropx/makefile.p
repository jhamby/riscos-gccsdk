--- Makefile.orig	2021-03-06 16:13:30.389309263 +0000
+++ Makefile	2021-03-06 16:15:14.304468963 +0000
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
+MIXER=-lSDL_mixer -logg -lvorbis -lvorbisfile -lmikmod -lFLAC
 NOSOUNDFLAG=$(SOUND)SOUND
 JOYFLAG=$(JOY)JOYSTICK
 EMBEDDEDFLAG=NOT_EMBEDDED
@@ -31,7 +32,7 @@
 
 CFLAGS=$(DEB_CFLAGS) $(SDL_CFLAGS) \
 	-DDATA_PREFIX=\"$(DATA_PREFIX)\" -D$(NOSOUNDFLAG) -D$(JOYFLAG) \
-	-D$(EMBEDDEDFLAG) $(XTRA_FLAGS)
+	-D$(EMBEDDEDFLAG) $(XTRA_FLAGS) -O2
 SDL_LIB=$(MIXER) $(SDL_LDFLAGS)
 CQML_LIB=-L./cqml -lcqml -L/opt/Qtopia/lib -lqpe -lqte
 #CQML_LIB=-L/opt/Qtopia/lib -lqpe -lqte
@@ -64,7 +65,7 @@
 
 
 gemdropx:	gemdropx.o
-	$(CXX) $(CFLAGS) $(CPPFLAGS) -o gemdropx gemdropx.o $(GFX_LIB) -lm $(LDFLAGS)
+	$(CXX) $(CFLAGS) $(CPPFLAGS) -o gemdropx gemdropx.o $(GFX_LIB) -lm $(LDFLAGS) -static
 
 
 gemdropx.o:	gemdropx.c
