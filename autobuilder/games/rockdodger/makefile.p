--- Makefile.orig	2010-05-28 11:30:36.000000000 +0100
+++ Makefile	2010-05-28 11:32:54.000000000 +0100
@@ -23,14 +23,16 @@
 SOUNDSOURCE=sound
 EXENAME=rockdodger
 
-override CFLAGS += -Wall `sdl-config --cflags` $(OPTIONS)
-LDFLAGS	=
+#override CFLAGS += -Wall `sdl-config --cflags` $(OPTIONS)
+CFLAGS += -Wall `sdl-config --cflags` $(OPTIONS)
+LDFLAGS	= -static
 
 #SOUNDLIBRARIES=-lvorbisfile -lvorbis -logg
 #SOUNDLIBRARIES=
-SOUNDLIBRARIES=-lSDL_mixer
+SOUNDLIBRARIES=-lSDL_mixer -lvorbisfile -lvorbis -logg
 
-LIBRARIES=-lSDL_image $(SOUNDLIBRARIES) `sdl-config --libs` -lm
+LIBRARIES= \
+           $(SOUNDLIBRARIES) -lSDL -lSDL_image -ljpeg -lpng -ltiff -lz
 
 all:	$(EXENAME)
 
