--- Makefile.orig	2007-08-10 14:40:48.353328000 +0100
+++ Makefile	2007-08-10 14:50:20.237328000 +0100
@@ -23,14 +23,18 @@
 SOUNDSOURCE=sound
 EXENAME=rockdodger
 
-override CFLAGS += -Wall `sdl-config --cflags` $(OPTIONS)
+#override CFLAGS += -Wall `sdl-config --cflags` $(OPTIONS)
+CFLAGS+= -Wall -I$(GCCSDK_INSTALL_ENV)/include $(OPTIONS) -O2
-LDFLAGS	=
+LDFLAGS	= -static
 
 #SOUNDLIBRARIES=-lvorbisfile -lvorbis -logg
 #SOUNDLIBRARIES=
-SOUNDLIBRARIES=-lSDL_mixer
+#SOUNDLIBRARIES=-lSDL_mixer
+SOUNDLIBRARIES=-lSDL_mixer -lvorbisfile -lvorbis -logg
 
-LIBRARIES=-lSDL_image $(SOUNDLIBRARIES) `sdl-config --libs`
+#LIBRARIES=-lSDL_image $(SOUNDLIBRARIES) `sdl-config --libs`
+LIBRARIES= -L$(GCCSDK_INSTALL_ENV)/lib -lSDL_image \
+           $(SOUNDLIBRARIES) -lSDL -lSDL_image -ljpeg -lpng -ltiff -lz
 
 all:	$(EXENAME)
 
