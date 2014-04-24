--- Makefile.orig	2014-04-11 13:28:36.000000000 +0100
+++ Makefile	2014-04-11 13:29:05.000000000 +0100
@@ -44,7 +44,7 @@
 OPTIONS=-DVERSION=\"$(VERSION)\" -DCOMPILEDATE=\"$(COMPILEDATE)\" -Wall `$(SDL_CONFIG) --cflags`
 EXENAME=rockdodger
 
-SOUNDLIBRARIES=-lSDL_mixer
+SOUNDLIBRARIES=-lSDL_mixer -lvorbisfile -lvorbis -logg -lmikmod -static
 SDL_CONFIG=sdl-config
 
 LIBRARIES=`$(SDL_CONFIG) --libs` -lSDL_image $(SOUNDLIBRARIES) -lm
